//
// Created by justin on 2021/02/07.
//

#ifndef J_TOOL_FFMPEGRECORDER_H
#define J_TOOL_FFMPEGRECORDER_H

extern "C" {
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
};

class FFmpegRecorder {

public:

    explicit FFmpegRecorder();

    ~FFmpegRecorder();

    int recordAudio();

private:

    int openDevice();

    int beginRecord();

    int record();

    int deallocate();

    char errorMessage[100];

    AVInputFormat *inputFormat;
    AVFormatContext *formatContext;

};


#endif //J_TOOL_FFMPEGRECORDER_H
