//
// Created by justin on 2021/01/20.
//

#include "FFmpegFrame.h"

#include <iostream>
#include <chrono>
#include <fstream>

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

int FFmpegFrame::initializeSwsContext(AVPixelFormat dstFormat) {
    pAVFrameRet = av_frame_alloc();
    int numBytes = av_image_get_buffer_size(dstFormat, pAVCodecContext->width, pAVCodecContext->height, 32);
    buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

    av_image_fill_arrays(
            pAVFrameRet->data,
            pAVFrameRet->linesize,
            buffer,
            dstFormat,
            pAVCodecContext->width,
            pAVCodecContext->height,
            32
    );
    pSwsContext = sws_getContext(
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

    return 0;
}

void FFmpegFrame::deallocateFFmpeg() {
    videoStreamIndex = -1;
    audioStreamIndex = -1;
    if (pAVFrame != nullptr) {
        av_frame_free(&pAVFrame);
    }
    if (pSwsContext != nullptr) {
        av_free(buffer);
        av_frame_free(&pAVFrameRet);
        sws_freeContext(pSwsContext);
    }
    avcodec_free_context(&pAVCodecContext);
    avformat_close_input(&pAVFormatContext);
}

int FFmpegFrame::scaleImage() {
    sws_scale(
            pSwsContext,
            (uint8_t const *const *) pAVFrame->data,
            pAVFrame->linesize,
            0,
            pAVCodecContext->height,
            pAVFrameRet->data,
            pAVFrameRet->linesize
    );
    return 0;
}

int FFmpegFrame::saveImage(AVFrame *pFrame, int width, int height, const std::string &diskPath) {
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    const std::string fullFilename = diskPath + "_screenshot_" + std::to_string(ms.count()) + ".ppm";
    std::ofstream ofs(fullFilename, std::ios_base::out | std::ios_base::binary);
    // write header
    ofs << "P6\n" << width << " " << height << "\n" << "255\n";
    // Write pixel data
    for (int y = 0; y < height; y++) {
        ofs.write((const char *) pFrame->data[0] + y * pFrame->linesize[0], width * 3);
    }

    ofs.close();
    return 0;
}

int FFmpegFrame::scaleAndSaveImage(AVFrame *pFrame, int width, int height, const std::string &diskPath) {
    scaleImage();
    saveImage(pFrame, width, height, diskPath);
    return 0;
}

int FFmpegFrame::decodeFramesAndSaveImages(int nFrames, AVPixelFormat dstFormat, const std::string &diskPath) {
    pAVFrame = av_frame_alloc();
    AVPacket packet;
    int ret = 0;
    int times = 0;

    if (pSwsContext == nullptr) initializeSwsContext(dstFormat);

    while (av_read_frame(pAVFormatContext, &packet) == 0) {
        if (++times == nFrames) break;

        if (packet.stream_index == videoStreamIndex) {
            ret = avcodec_send_packet(pAVCodecContext, &packet);
            if (ret != 0) {
                av_strerror(ret, errorMessage, sizeof(errorMessage));
                std::cout << "\navcodec_send_packet: " << errorMessage;
                return ret;
            }

            ret = avcodec_receive_frame(pAVCodecContext, pAVFrame);
            if (ret != 0) {
                av_strerror(ret, errorMessage, sizeof(errorMessage));
                std::cout << "\navcodec_receive_frame: " << errorMessage;
                return ret;
            }

            scaleAndSaveImage(pAVFrameRet, pAVCodecContext->width, pAVCodecContext->height, diskPath);
        }
        av_packet_unref(&packet);
    }

    return ret;
}

int FFmpegFrame::decodeFrameAndSaveImages(double targetSeconds, AVPixelFormat dstFormat, const std::string &diskPath) {
    pAVFrame = av_frame_alloc();
    AVPacket packet;
    int ret = 0;

    if (pSwsContext == nullptr) initializeSwsContext(dstFormat);

    int64_t streamDuration = pAVFormatContext->streams[videoStreamIndex]->duration;
    double streamTimeBase = av_q2d(pAVFormatContext->streams[videoStreamIndex]->time_base);

    if (targetSeconds >= streamDuration * streamTimeBase) {
        std::cout << "\ntarget seconds cannot greater than total seconds of video stream";
        return -1;
    }

    if (av_seek_frame(pAVFormatContext, videoStreamIndex, targetSeconds / streamTimeBase, AVSEEK_FLAG_BACKWARD) < 0) {
        std::cout << "\nseek frame failed";
        return -1;
    }

    bool done = false;
    while (!done && av_read_frame(pAVFormatContext, &packet) == 0) {

        if (packet.stream_index == videoStreamIndex) {
            ret = avcodec_send_packet(pAVCodecContext, &packet);
            if (ret != 0) continue;

            ret = avcodec_receive_frame(pAVCodecContext, pAVFrame);
            if (ret == AVERROR_EOF) {
                av_strerror(ret, errorMessage, 1024);
                std::cout << "receive frame failed: " << errorMessage << std::endl;
                return ret;
            }

            std::cout << "\npacketSeconds: " << (packet.pts * streamTimeBase);
            if (pAVFrame->width > 0) {
                done = true;
            }

            scaleAndSaveImage(pAVFrameRet, pAVCodecContext->width, pAVCodecContext->height, diskPath);
        }
        av_packet_unref(&packet);
    }

//    av_seek_frame(pAVFormatContext, videoStreamIndex, 0, AVSEEK_FLAG_BYTE);
    avcodec_flush_buffers(pAVCodecContext);

    return ret;
}

int FFmpegFrame::saveFrameImage(const std::string &filepath, double targetSeconds, int nFrames, AVPixelFormat dstFormat,
                                const std::string &diskPath) {
    if (initializeFFmpeg(filepath) < 0) {
        std::cout << "\ninitializeFFmpeg failed";
        return -1;
    }
    if (targetSeconds == -1 && nFrames > 0) {
        if (decodeFramesAndSaveImages(nFrames, dstFormat, diskPath) < 0) {
            std::cout << "\ndecodeFramesAndSaveImages failed";
            return -1;
        }
    } else {
        if (decodeFrameAndSaveImages(targetSeconds, dstFormat, diskPath) < 0) {
            std::cout << "\ndecodeFrameInTargetSeconds failed";
            return -1;
        }
    }
    deallocateFFmpeg();
    return 0;
}
