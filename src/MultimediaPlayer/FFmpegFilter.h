//
// Created by justin on 2021/01/28.
//

#pragma once

#include <string>
#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>
}

enum AVPixelFormat pix_fmts[] = {AV_PIX_FMT_GRAY8, AV_PIX_FMT_NONE};
const char *filter_descr = "scale=78:24,transpose=cclock";

class FFmpegFilter {

public:

    explicit FFmpegFilter();

    ~FFmpegFilter();

    void test();

    int initializeOpenFile(const std::string &filepath);

    int initializeFilter(const char *filtersDescr);

    int decodeFilterFrames(const std::string &filepath, int nFrames);

    void deallocateInOut();

    void deallocate();

private:

    int64_t last_pts = AV_NOPTS_VALUE;

    AVFormatContext *formatContext;
    AVCodec *codec;
    AVCodecContext *codecContext;

    AVFilterGraph *filterGraph;
    const AVFilter *buffersrc;
    const AVFilter *buffersink;
    AVFilterContext *buffersrcContext;
    AVFilterContext *buffersinkContext;
    AVFilterInOut *inputs;
    AVFilterInOut *outputs;

    AVFrame *frame;
    AVFrame *filterFrame;

    int videoIndexStream = -1;

};
