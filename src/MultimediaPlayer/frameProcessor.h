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

typedef struct FileInfo {
    std::string filename;
    int duration;
    int bit_rate;
} FileInfo;

int savePixelToDisk(AVFrame *pFrame);

int saveFrameAsPicture(AVCodecContext *pCodecCtx, AVFrame *pFrame, AVPixelFormat dstFormat);

int getFrameWithTimestamp(AVFrame *pFrame, AVFormatContext *pFormatCtx, AVCodecContext *pCodecCtx, int videoStreamIndex, int64_t timestamp);

int getPixmapWithTimestamp(const std::string &filename, int64_t timestamp);

int initializeFFmpeg(const std::string &filename);

int freeFFmpegStruct(void *args[]);
