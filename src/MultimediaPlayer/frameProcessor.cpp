//
// Created by justin on 2021/01/08.
//

#include <string>
#include <iostream>
#include <stdexcept>
#include <any>
#include <functional>

#include "./frameProcessor.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

void saveFrame(AVFrame *avFrame, int width, int height) {
    FILE *pFile;
    char szFilename[32];
    int y;

    // Open file
    sprintf(szFilename, "frame%d.ppm");
    pFile = fopen(szFilename, "wb");
    if (pFile == NULL) {
        return;
    }

    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    // Write pixel data
    for (y = 0; y < height; y++) {
        fwrite(avFrame->data[0] + y * avFrame->linesize[0], 1, width * 3, pFile);
    }

    // Close file
    fclose(pFile);
}

int saveFrameAsPicture(AVCodecContext *pCodecCtx, AVFrame *pFrame, AVPixelFormat dstFormat) {
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
    saveFrame(pFrameRet, pCodecCtx->width, pCodecCtx->height);

    av_free(buffer);
    av_frame_free(&pFrameRet);
    sws_freeContext(swsCtx);

    return 0;
}

int getFrameWithTimestamp(AVFrame *pFrame, AVFormatContext *pFormatCtx, AVCodecContext *pCodecCtx, int videoStreamIndex,
                          int targetSeconds) {
    AVPacket packet;
    int ret = 0;

    double streamTimeBase = av_q2d(pFormatCtx->streams[videoStreamIndex]->time_base);
    std::cout << "AVStream->time_base: " << streamTimeBase << std::endl;

    if (av_seek_frame(pFormatCtx, videoStreamIndex, targetSeconds / streamTimeBase, AVSEEK_FLAG_BACKWARD) < 0) {
        std::cout << "seek frame failed" << std::endl;
        return -1;
    }

    bool done = false;
    int read_frame_times = 0;
    while (!done && av_read_frame(pFormatCtx, &packet) >= 0) {

        if (packet.stream_index == videoStreamIndex) {
            std::cout << "read_frame_times: " << ++read_frame_times << std::endl;
            ret = avcodec_send_packet(pCodecCtx, &packet);
            if (ret < 0) {
                std::cout << "send packet failed" << std::endl;
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

int getPixmapWithTimestamp(const std::string &filename, int targetSeconds) {
    std::cout << "Current MediaFile: " << filename << std::endl;

    AVFormatContext *pFormatCtx = nullptr;
    int ret = 0;

    const char *url = filename.c_str();
    ret = avformat_open_input(&pFormatCtx, url, nullptr, nullptr);
    if (ret < 0) {
        std::cout << "cannot open file" << std::endl;
        return -1;
    }

    ret = avformat_find_stream_info(pFormatCtx, nullptr);
    if (ret < 0) {
        std::cout << "cannot find stream info" << std::endl;
        return -1;
    }

    // find index of video stream
    int videoStreamIndex = -1;
    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        AVCodecParameters *codecParameters = pFormatCtx->streams[i]->codecpar;
        if (codecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }
    if (videoStreamIndex == -1) {
        std::cout << "cannot find video stream" << std::endl;
        return -1;
    }

    AVCodecContext *pCodecCtx = nullptr;
    AVCodec *pCodec = nullptr;
    pCodec = avcodec_find_decoder(pFormatCtx->streams[videoStreamIndex]->codecpar->codec_id);
    if (pCodec == nullptr) {
        std::cout << "cannot find decoder" << std::endl;
        return -1;
    }

    // copy context
    pCodecCtx = avcodec_alloc_context3(pCodec);
    ret = avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStreamIndex]->codecpar);
    if (ret < 0) {
        std::cout << "cannot fill codec context" << std::endl;
        return -1;
    }

    // initialize codec context to use given codec
    ret = avcodec_open2(pCodecCtx, pCodec, nullptr);
    if (ret < 0) {
        std::cout << "cannot initialize codec context to use given codec" << std::endl;
        return -1;
    }

    AVFrame *pFrame = av_frame_alloc();
    getFrameWithTimestamp(pFrame, pFormatCtx, pCodecCtx, videoStreamIndex, targetSeconds);

    std::cout << "Frame Width: " << std::to_string(pFrame->width) << std::endl;
    std::cout << "Frame Height: " << std::to_string(pFrame->height) << std::endl;

    saveFrameAsPicture(pCodecCtx, pFrame, AV_PIX_FMT_RGB24);

    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecCtx);
    avformat_close_input(&pFormatCtx);
    return 0;
}

int initializeFFmpeg(const std::string &filename) {
    AVFormatContext *pFormatCtx = nullptr;
    int ret = 1;

    const char *url = filename.c_str();
    ret = avformat_open_input(&pFormatCtx, url, nullptr, nullptr);
    if (ret < 0) {
        std::cout << "cannot open file" << std::endl;
        return -1;
    }

    ret = avformat_find_stream_info(pFormatCtx, nullptr);
    if (ret < 0) {
        std::cout << "cannot find stream info" << std::endl;
        return -1;
    }

    // todo

    avformat_close_input(&pFormatCtx);
    return 0;
}

int freeFFmpeg(void *arg[]) {
    // todo free all ffmpeg structs
    return 0;
}
