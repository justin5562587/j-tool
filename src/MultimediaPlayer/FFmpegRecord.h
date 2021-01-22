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

    /**
     * initialize record device
     */
    int initializeRecordDevice();

    /**
     * initialize FFmpeg structs (eg. AVFormatContext, encoder) for outputFile
     */
    int initializeOutputFile(AVCodecID avCodecId, const std::string &outputFilePath);

    /**
     * collect data from record device, scale data, then encode and write data to outputFile
     */
    int captureVideoFrames();

    /**
     * clean all FFmpeg structs
     */
    int cleanAll();

    int recordVideo(AVCodecID avCodecId, const std::string &outputFilePath);

private:

    char errorMessage[1024];

    AVInputFormat *pAVInputFormat;
    AVFormatContext *pAVFormatContext;
    AVCodecContext *pAVCodecContext;
    AVFrame *pAVFrame;
    AVCodec *pAVCodec;

    AVPacket *pAVPacket;

    AVDictionary *options;

    AVOutputFormat *outAVOutputFormat;
    AVFormatContext *outAVFormatContext;
    AVCodecContext *outAVCodecContext;
    AVFrame *outFrame;
    AVCodec *outAVCodec;

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
