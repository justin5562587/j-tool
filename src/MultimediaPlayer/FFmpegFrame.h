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
     * initialize AVFormatContext, AVCodecContext, AVCodec with file in filepath and open Codec
     */
    int initializeFFmpeg(const std::string &filepath);

    /**
     * clean AVFormatContext, AVCodecContext, AVCodec and etc.
     */
    void deallocateFFmpeg();

    /**
     * write image into disk file
     */
    int saveImage(AVFrame *pFrame, int width, int height, const std::string &diskPath);

    /**
     * Public API
     * decode video stream with conditions
     * seek and decode frame in targetSeconds and export picture
     */
    int decodeFrameAndSaveImages(double targetSeconds, AVPixelFormat dstFormat, const std::string &diskPath);

    /**
     * Public API
     * decode from beginning of video stream
     * decode nFrames of frames and export pictures
     */
    int decodeFramesAndSaveImages(int nFrames, AVPixelFormat dstFormat, const std::string &diskPath);

    /**
     * Public API
     * process of whole FFmpegFrame
     */
    int saveFrameImage(const std::string &filepath, double targetSeconds, int nFrames, AVPixelFormat dstFormat, const std::string &diskPath);

private:

    char errorMessage[1024];

    AVFormatContext *pAVFormatContext = nullptr;
    AVCodec *pAVCodec = nullptr;
    AVCodecContext *pAVCodecContext = nullptr;
    AVFrame *pAVFrame = nullptr;
    AVFrame *pAVFrameRet = nullptr;

    int videoStreamIndex;
    int audioStreamIndex;

};
