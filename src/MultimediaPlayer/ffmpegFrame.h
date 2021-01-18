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

/**
 * custom struct wrapping some useful fields taken from ffmpeg
 */
struct FFmpegBasicInfo {
    int videoStreamIndex = -1;
    int audioStreamIndex = -1;
};

/**
 * populate AVFormatContext and FFmpegBasicInfo with given filepath
 */
int initializeFFmpeg(AVFormatContext *pFormatCtx, FFmpegBasicInfo *ffmpegBasicInfo, const std::string &filepath);

/**
 * populate AVCodecContext and open Codec with given AVFormatContext and FFmpegBasicInfo
 */
int initializeCodec(AVCodecContext **ppCodecCtx, AVFormatContext *pFormatCtx, FFmpegBasicInfo *ffmpegBasicInfo);

/**
 * deallocate ffmpeg relevant structs
 */
int deallocateFFmpeg(AVFormatContext *pFormatCtx, AVCodecContext *pCodecCtx);

/**
 * write scaled format frame data to disk file
 */
void writeFrameToDiskFile(AVFrame *avFrame, int width, int height, const std::string &diskPath);

/**
 * get frame data using avformat_seek_frame
 */
int getFrameInSpecificSeconds(AVFrame *pFrame, AVFormatContext *pFormatCtx, AVCodecContext *pCodecCtx, int videoStreamIndex, double targetSeconds);

/**
 * get a pixel picture according to video filepath and saved diskPath
 */
int getPixmapInSpecificSeconds(const std::string &filepath, double targetSeconds, const std::string &diskPath);

// todo
int getVideoOverviewPicture(const std::string &filepath);
