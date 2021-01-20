//
// Created by justin on 2021/01/20.
//

#include "FFmpegFrame.h"

#include <iostream>
#include <chrono>
#include <fstream>

void writeFrameToFile(AVFrame *pFrame, int width, int height, const std::string &diskPath) {
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

FFmpegFrame::FFmpegFrame() {
}

FFmpegFrame::~FFmpegFrame() {
}

int FFmpegFrame::initializeFFmpeg(const std::string &filepath) {
    avformat_open_input(&pAVFormatContext, filepath.c_str(), nullptr, nullptr);

    videoStreamIndex = -1;
    for (int i = 0; i < pAVFormatContext->nb_streams; ++i) {
        if (pAVFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
        }
        if (pAVFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStreamIndex = i;
        }
    }

    if (videoStreamIndex == -1) {
        std::cout << "\ncannot find video stream";
        return -1;
    }

    // find video decoder
    pAVCodec = avcodec_find_decoder(pAVFormatContext->streams[videoStreamIndex]->codecpar->codec_id);
    pAVCodecContext = avcodec_alloc_context3(pAVCodec);
    avcodec_parameters_to_context(pAVCodecContext, pAVFormatContext->streams[videoStreamIndex]->codecpar);
    avcodec_open2(pAVCodecContext, pAVCodec, nullptr);

    return 0;
}

void FFmpegFrame::deallocateFFmpeg() {
    videoStreamIndex = -1;
    audioStreamIndex = -1;
    av_frame_free(&pAVFrame);
    avcodec_free_context(&pAVCodecContext);
    avformat_close_input(&pAVFormatContext);
}

int FFmpegFrame::getFrameInTargetSeconds(double targetSeconds) {
    pAVFrame = av_frame_alloc();
    AVPacket packet;
    int ret = 0;

    int64_t streamDuration = pAVFormatContext->streams[videoStreamIndex]->duration;
    double streamTimeBase = av_q2d(pAVFormatContext->streams[videoStreamIndex]->time_base);

    if (targetSeconds >= streamDuration * streamTimeBase) {
        std::cout << "target seconds cannot greater than total seconds of video stream " << std::endl;
        return -1;
    }

    if (av_seek_frame(pAVFormatContext, videoStreamIndex, targetSeconds / streamTimeBase, AVSEEK_FLAG_BACKWARD) < 0) {
        std::cout << "seek frame failed" << std::endl;
        return -1;
    }

    bool done = false;
    int send_packet_times = 0;
    while (!done && av_read_frame(pAVFormatContext, &packet) == 0) {

        if (packet.stream_index == videoStreamIndex) {
            ret = avcodec_send_packet(pAVCodecContext, &packet);
            std::cout << "avcodec_send_packet_times: " << ++send_packet_times << std::endl;
            if (ret < 0) {
                std::cout << "send packet failed\n";
                return -1;
            } else {
                ret = avcodec_receive_frame(pAVCodecContext, pAVFrame);
//                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
//                    char errBuffer[128];
//                    av_make_error_string(errBuffer, sizeof(errBuffer), ret);
//                    std::cout << "receive frame failed: " << errBuffer << std::endl;
//                    return -1;
//                }

                double packetSeconds = packet.pts * streamTimeBase;
                std::cout << "packetSeconds: " << packetSeconds << std::endl;

                if (pAVFrame->width > 0) {
                    done = true;
                }
            }
        }
        av_packet_unref(&packet);
    }

//    av_seek_frame(pAVFormatContext, videoStreamIndex, 0, AVSEEK_FLAG_BYTE);
//    avcodec_flush_buffers(pCodecCtx);

    return 0;
}

int FFmpegFrame::scaleAndSaveToImage(AVPixelFormat dstFormat, const std::string &diskPath, bool cleanAll) {
    AVFrame *pFrameRet = av_frame_alloc();
    int numBytes = av_image_get_buffer_size(dstFormat, pAVCodecContext->width, pAVCodecContext->height, 32);
    uint8_t *buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(
            pFrameRet->data,
            pFrameRet->linesize,
            buffer,
            dstFormat,
            pAVCodecContext->width,
            pAVCodecContext->height,
            32
    );

    struct SwsContext *swsCtx = sws_getContext(
            pAVCodecContext->width,
            pAVCodecContext->height,
            pAVCodecContext->pix_fmt,
            pAVCodecContext->width,
            pAVCodecContext->height,
            dstFormat,
            SWS_BICUBIC,
            nullptr,
            nullptr,
            nullptr
    );
    sws_scale(
            swsCtx,
            (uint8_t const *const *) pAVFrame->data,
            pAVFrame->linesize,
            0,
            pAVCodecContext->height,
            pFrameRet->data,
            pFrameRet->linesize
    );

    // save file to disk
    writeFrameToFile(pFrameRet, pAVCodecContext->width, pAVCodecContext->height, diskPath);

    av_free(buffer);
    av_frame_free(&pFrameRet);
    sws_freeContext(swsCtx);

    if (cleanAll) {
        this->deallocateFFmpeg();
    }

    return 0;
}



