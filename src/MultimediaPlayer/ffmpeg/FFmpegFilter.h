//
// Created by justin on 2021/01/28.
//

#pragma once

#include <string>
#include <iostream>
#include <fstream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>
}

class FFmpegFilter {

public:

    explicit FFmpegFilter();

    ~FFmpegFilter();

    void test();

    int initializeOpenFile(const std::string &filepath);

    int initializeFilter(const char *filtersDescr);

    int decodeFilterFrames(const std::string &filepath, int nFrames, const std::string &diskPath);

    void deallocateInOut();

    void deallocate();

private:

    int64_t last_pts = AV_NOPTS_VALUE;

    char errorMessage[512];

    AVFormatContext *formatContext = nullptr;
    AVCodec *codec = nullptr;
    AVCodecContext *codecContext = nullptr;

    AVFilterGraph *filterGraph = nullptr;
    const AVFilter *buffersrc = nullptr;
    const AVFilter *buffersink = nullptr;
    AVFilterContext *buffersrcContext = nullptr;
    AVFilterContext *buffersinkContext = nullptr;
    AVFilterInOut *inputs = nullptr;
    AVFilterInOut *outputs = nullptr;

    AVFrame *frame;
    AVFrame *filterFrame;

    int videoIndexStream = -1;

};
