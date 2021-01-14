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

/**
 * populate AVFormatContext and FFmpegBasicInfo with given filepath
 */
int initializeFFmpeg(AVFormatContext *pFormatCtx, FFmpegBasicInfo *ffmpegBasicInfo, const std::string &filepath);

/**
 * deallocate ffmpeg relevant structs
 */
int deallocateFFmpeg(AVFormatContext *pFormatCtx, AVCodecContext *pCodecCtx);

/**
 * populate AVCodecContext and open Codec with given AVFormatContext and FFmpegBasicInfo
 */
int initializeCodec(AVCodecContext **ppCodecCtx, AVFormatContext *pFormatCtx, FFmpegBasicInfo *ffmpegBasicInfo);

int savePixelToDisk(AVFrame *pFrame);

int saveFrameAsPicture(AVCodecContext *pCodecCtx, AVFrame *pFrame, AVPixelFormat dstFormat);

int getFrameInSpecificSeconds(AVFrame *pFrame, AVFormatContext *pFormatCtx, AVCodecContext *pCodecCtx, int videoStreamIndex, int targetSeconds);

int getPixmapInSpecificSeconds(const std::string &filepath, int targetSeconds);

int getVideoOverviewPicture(const std::string &filepath);
