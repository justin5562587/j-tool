//
// Created by justin on 2021/01/08.
//

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <chrono>
#include <string>

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "./frameProcessor.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

int initializeFFmpeg(AVFormatContext *pFormatCtx, FFmpegBasicInfo *ffmpegBasicInfo, const std::string &filepath) {
    std::cout << "Current MediaFile: " << filepath << std::endl;
    int ret = 0;

    const char *url = filepath.c_str();
    if (avformat_open_input(&pFormatCtx, url, nullptr, nullptr) < 0) {
        std::cout << "cannot open file" << std::endl;
        return -1;
    }

    if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) {
        std::cout << "cannot find stream info" << std::endl;
        return -1;
    }

    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        AVCodecParameters *codecParameters = pFormatCtx->streams[i]->codecpar;
        if (codecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            ffmpegBasicInfo->videoStreamIndex = i;
        }
        if (codecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {
            ffmpegBasicInfo->audioStreamIndex = i;
        }
    }

    int videoStreamIndex = ffmpegBasicInfo->videoStreamIndex;
    if (videoStreamIndex == -1) {
        std::cout << "cannot find video stream" << std::endl;
        return -1;
    }

    return 0;
}

int initializeCodec(AVCodecContext **ppCodecCtx, AVFormatContext *pFormatCtx, FFmpegBasicInfo *pFFmpegBasicInfo) {
    int videoStreamIndex = pFFmpegBasicInfo->videoStreamIndex;
    // find Codec
    AVCodec *pCodec = avcodec_find_decoder(pFormatCtx->streams[videoStreamIndex]->codecpar->codec_id);
    if (pCodec == nullptr) {
        std::cout << "cannot find decoder" << std::endl;
        return -1;
    }
    // initialize CodecContext with found Codec
    *ppCodecCtx = avcodec_alloc_context3(pCodec);
    if (avcodec_parameters_to_context(*ppCodecCtx, pFormatCtx->streams[videoStreamIndex]->codecpar) < 0) {
        std::cout << "cannot fill CodecContext with given Codec" << std::endl;
        return -1;
    }
    if (avcodec_open2(*ppCodecCtx, pCodec, nullptr) < 0) {
        std::cout << "cannot initialize CodecContext to use given Codec" << std::endl;
        return -1;
    }

    return 0;
}

int deallocateFFmpeg(AVFormatContext *pFormatCtx, AVCodecContext *pCodecCtx) {
    avcodec_free_context(&pCodecCtx);
    avformat_close_input(&pFormatCtx);
    return 0;
}

void writeFrameToDiskFile(AVFrame *pFrame, int width, int height, const std::string &diskPath) {
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    const std::string fullFilename = diskPath + "_screenshot_" + std::to_string(ms.count()) + ".ppm";
    std::ofstream ofs(fullFilename, std::ios_base::out | std::ios_base::binary);
    // write header
    ofs << "P6\n" << width << " " << height << "\n" << "255\n";
    // Write pixel data
    for (int y = 0; y < height; y++) {
        ofs.write((const char*) pFrame->data[0] + y * pFrame->linesize[0], width * 3);
    }

    ofs.close();
}

void writeFrameToDiskFileC(AVFrame *avFrame, int width, int height, const std::string &diskPath) {
    char szFilename[64];
    // Open file
    sprintf(szFilename, "%s_screenshot.ppm", diskPath.c_str());
    FILE *pFile = fopen(szFilename, "wb");

    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);
    // Write pixel data
    for (int y = 0; y < height; y++) {
        fwrite(avFrame->data[0] + y * avFrame->linesize[0], 1, width * 3, pFile);
    }

    // Close file
    fclose(pFile);
}

int
saveFrameAsPicture(AVCodecContext *pCodecCtx, AVFrame *pFrame, AVPixelFormat dstFormat, const std::string &diskPath) {
    AVFrame *pFrameRet = av_frame_alloc();
    int numBytes = av_image_get_buffer_size(dstFormat, pCodecCtx->width, pCodecCtx->height, 32);
    uint8_t *buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(
            pFrameRet->data,
            pFrameRet->linesize,
            buffer,
            dstFormat,
            pCodecCtx->width,
            pCodecCtx->height,
            32
    );

    struct SwsContext *swsCtx = sws_getContext(
            pCodecCtx->width,
            pCodecCtx->height,
            pCodecCtx->pix_fmt,
            pCodecCtx->width,
            pCodecCtx->height,
            dstFormat,
            SWS_BILINEAR,
            nullptr,
            nullptr,
            nullptr
    );
    sws_scale(
            swsCtx,
            (uint8_t const *const *) pFrame->data,
            pFrame->linesize,
            0,
            pCodecCtx->height,
            pFrameRet->data,
            pFrameRet->linesize
    );

    // save file to disk
    writeFrameToDiskFile(pFrameRet, pCodecCtx->width, pCodecCtx->height, diskPath);

    av_free(buffer);
    av_frame_free(&pFrameRet);
    sws_freeContext(swsCtx);

    return 0;
}

int
getFrameInSpecificSeconds(AVFrame *pFrame, AVFormatContext *pFormatCtx, AVCodecContext *pCodecCtx, int videoStreamIndex,
                          double targetSeconds) {
    AVPacket packet;
    int ret = 0;

    int64_t streamDuration = pFormatCtx->streams[videoStreamIndex]->duration;
    double streamTimeBase = av_q2d(pFormatCtx->streams[videoStreamIndex]->time_base);

    if (targetSeconds >= streamDuration * streamTimeBase) {
        std::cout << "target seconds cannot greater than total seconds of video stream " << std::endl;
        return -1;
    }

    if (av_seek_frame(pFormatCtx, videoStreamIndex, targetSeconds / streamTimeBase, AVSEEK_FLAG_BACKWARD) < 0) {
        std::cout << "seek frame failed" << std::endl;
        return -1;
    }

    bool done = false;
    int send_packet_times = 0;
    while (!done && av_read_frame(pFormatCtx, &packet) >= 0) {

        if (packet.stream_index == videoStreamIndex) {
            ret = avcodec_send_packet(pCodecCtx, &packet);
            std::cout << "avcodec_send_packet_times: " << ++send_packet_times << std::endl;
            if (ret < 0) {
                std::cout << "send packet failed\n";
                return -1;
            } else {
                ret = avcodec_receive_frame(pCodecCtx, pFrame);
//                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
//                    char errBuffer[128];
//                    av_make_error_string(errBuffer, sizeof(errBuffer), ret);
//                    std::cout << "receive frame failed: " << errBuffer << std::endl;
//                    return -1;
//                }

                double packetSeconds = packet.pts * streamTimeBase;
                std::cout << "packetSeconds: " << packetSeconds << std::endl;

                if (pFrame->width > 0) {
                    done = true;
                }
            }
        }
        av_packet_unref(&packet);
    }

//    av_seek_frame(pFormatCtx, videoStreamIndex, 0, AVSEEK_FLAG_BYTE);
//    avcodec_flush_buffers(pCodecCtx);
    return 0;
}

int getPixmapInSpecificSeconds(const std::string &filepath, double targetSeconds, const std::string &diskPath) {
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    FFmpegBasicInfo ffmpegBasicInfo;
    initializeFFmpeg(pFormatCtx, &ffmpegBasicInfo, filepath);

    AVCodecContext *pCodecCtx = nullptr;
    initializeCodec(&pCodecCtx, pFormatCtx, &ffmpegBasicInfo);

    int videoStreamIndex = ffmpegBasicInfo.videoStreamIndex;

    AVFrame *pFrame = av_frame_alloc();

    getFrameInSpecificSeconds(pFrame, pFormatCtx, pCodecCtx, videoStreamIndex, targetSeconds);

    saveFrameAsPicture(pCodecCtx, pFrame, AV_PIX_FMT_RGB24, diskPath);

    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecCtx);
    avformat_close_input(&pFormatCtx);
    return 0;
}

// todo - video overview picture
int getVideoOverviewPicture(const std::string &filepath) {

    return 0;
}
