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
}

int getInfoAboutFile(AVFormatContext *pFormatCtx, std::map<std::string, std::string> *fileInfo) {
    fileInfo->insert(std::make_pair("duration", std::to_string(pFormatCtx->duration)));
    fileInfo->insert(std::make_pair("bit_rate", std::to_string(pFormatCtx->bit_rate)));

    return 0;
}

int getFrameWithTimestamp(AVFrame *pFrame, AVFormatContext *pFormatCtx, AVCodecContext *pCodecCtx, int videoStreamIndex,
                          int64_t timestamp) {
    AVPacket packet;
    int ret = 0;

    if (av_seek_frame(pFormatCtx, videoStreamIndex, timestamp * AV_TIME_BASE, AVSEEK_FLAG_BACKWARD) < 0) {
        std::cout << "seek frame failed" << std::endl;
        return -1;
    }

    bool done = false;
    while (!done && av_read_frame(pFormatCtx, &packet) >= 0) {
        if (packet.stream_index == videoStreamIndex) {
            ret = avcodec_send_packet(pCodecCtx, &packet);
            if (ret < 0) {
                std::cout << "send packet failed" << std::endl;
                return -1;
            } else {
                ret = avcodec_receive_frame(pCodecCtx, pFrame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF || ret < 0) {
                    std::cout << "receive frame failed" << std::endl;
                    return -1;
                }

                if (pFrame->width != 0) {
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

int getPixmapWithTimestamp(const std::string &filename, int64_t timestamp) {
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

    // video stream is required
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
    ret = getFrameWithTimestamp(pFrame, pFormatCtx, pCodecCtx, videoStreamIndex, timestamp);
    if (ret < 0) {
        std::cout << "getFrameWithTimestamp failed" << std::endl;
        return -1;
    }

    std::cout << "Frame Width: " << std::to_string(pFrame->width) << std::endl;
    std::cout << "Frame Height: " << std::to_string(pFrame->height) << std::endl;

    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecCtx);
    avformat_close_input(&pFormatCtx);
    return 0;
}

int saveFrameAsPicture(AVCodecContext *pCodecCtx, AVFrame *pFrame) {
    // initialize sws
    AVFrame *pFrameRet = av_frame_alloc();

    struct SwsContext *swsCtx = sws_getContext(
            pCodecCtx->width,
            pCodecCtx->height,
            pCodecCtx->pix_fmt,
            pCodecCtx->width,
            pCodecCtx->height,
            AV_PIX_FMT_RGB24,
            SWS_BILINEAR,
            nullptr,
            nullptr,
            nullptr
    );




    // save file to disk


    return 0;
}

void saveFrame(AVFrame *avFrame, int width, int height, int frameIndex) {
    FILE *pFile;
    char szFilename[32];
    int y;

    // Open file
    sprintf(szFilename, "frame%d.ppm", frameIndex);
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


int initializeFFmpegWrapper(const std::string &filename, const std::string &callbackFuncName) {
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
