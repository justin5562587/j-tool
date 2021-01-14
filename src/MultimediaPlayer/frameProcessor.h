//
// Created by justin on 2021/01/08.
//
#pragma once

#include <string>
#include <map>

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

struct FFmpegBasicInfo {
    int videoStreamIndex = -1;
    int audioStreamIndex = -1;
};

int initializeFFmpeg(AVFormatContext *pFormatCtx, FFmpegBasicInfo *ffmpegBasicInfo, const std::string &filepath);

int deallocateFFmpeg(AVFormatContext *pFormatCtx, AVCodecContext *pCodecCtx, AVCodec *pCodec);

int savePixelToDisk(AVFrame *pFrame);

int saveFrameAsPicture(AVCodecContext *pCodecCtx, AVFrame *pFrame, AVPixelFormat dstFormat);

int getFrameInSpecificSeconds(AVFrame *pFrame, AVFormatContext *pFormatCtx, AVCodecContext *pCodecCtx, int videoStreamIndex, int targetSeconds);

int getPixmapInSpecificSeconds(const std::string &filepath, int targetSeconds);
