//
// Created by justin on 2021/01/28.
//

#include "FFmpegFilter.h"

void FFmpegFilter::test() {

    AVFilterGraph *filter_graph = avfilter_graph_alloc();

    // source filter
    char args[512];

    const AVFilter *bufferSrc = avfilter_get_by_name("buffer");
    AVFilterContext *bufferSrc_ctx = nullptr;
    avfilter_graph_create_filter(&bufferSrc_ctx, bufferSrc, "in", args, nullptr, filter_graph);

    // sink filter
    avfilter_inout_alloc();

}
