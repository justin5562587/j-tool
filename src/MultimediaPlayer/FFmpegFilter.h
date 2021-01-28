//
// Created by justin on 2021/01/28.
//

#pragma once

extern "C" {
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
}

class FFmpegFilter {

public:

    explicit FFmpegFilter();

    ~FFmpegFilter();

    void test();

private:


};
