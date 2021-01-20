//
// Created by justin on 2021/01/20.
//

#pragma once

#include <string>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

class FFmpegFrame {
public:

    FFmpegFrame();

    ~FFmpegFrame();

    /**
     * initialize AVFormatContext, AVCodecContext, AVCodec with file in filepath
     */
    int initializeFFmpeg(const std::string &filepath);

    /**
     * clean AVFormatContext, AVCodecContext, AVCodec and etc.
     */
    void deallocateFFmpeg();

    /**
     * write frame data into file
     */
    void writeFrameToFile(AVFrame *pFrame, int width, int height, const std::string &diskPath);

    /**
     * decode video stream and save data into frame in specific seconds
     */
    int getFrameInTargetSeconds(double targetSeconds);

    /**
     * save frame as picture
     */
    int scaleImage(AVPixelFormat dstFormat);

private:

    AVFormatContext *pAVFormatContext;
    AVCodec *pAVCodec;
    AVCodecContext *pAVCodecContext;
    AVFrame *pAVFrame;

    int videoStreamIndex;
    int audioStreamIndex;

};
