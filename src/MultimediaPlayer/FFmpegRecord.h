//
// Created by justin on 2021/01/19.
//

#pragma once

#include <iostream>

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavdevice/avdevice.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

class FFmpegRecord {

public:

    FFmpegRecord();

    ~FFmpegRecord();

    int openCamera();

    int initializeOutputFile();

    int captureVideoFrames();

private:

    AVInputFormat *pAVInputFormat;
    AVOutputFormat *output_format;

    AVCodecContext *pAVCodecContext;

    AVFormatContext *pAVFormatContext;

    AVFrame *pAVFrame;
    AVFrame *outFrame;

    AVCodec *pAVCodec;
    AVCodec *outAVCodec;

    AVPacket *pAVPacket;

    AVDictionary *options;

    AVOutputFormat *outAVOutputFormat;
    AVFormatContext *outAVFormatContext;
    AVCodecContext *outAVCodecContext;

    AVStream *video_st;
    AVFrame *outAVFrame;

    const char *deviceName;
    const char *outputFile;

    double videoPts;

    int outSize;
    int codecId;
    int value;
    int videoStreamIndex;

};
