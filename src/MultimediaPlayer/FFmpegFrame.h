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

    explicit FFmpegFrame();

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
     * seek frame in target seconds and decode data
     */
    int getFrameInTargetSeconds(double targetSeconds);

    /**
     * scale from original data to specific AVPixelFormat data and export to image picture
     */
    int scaleAndSaveImage(AVPixelFormat dstFormat, const std::string &diskPath, bool cleanAll);

    int saveFrameImage(const std::string &filepath, double targetSeconds, AVPixelFormat dstFormat, const std::string &diskPath);

private:

    AVFormatContext *pAVFormatContext = nullptr;
    AVCodec *pAVCodec = nullptr;
    AVCodecContext *pAVCodecContext = nullptr;
    AVFrame *pAVFrame = nullptr;

    int videoStreamIndex;
    int audioStreamIndex;

};
