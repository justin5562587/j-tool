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

enum RecordContent {
    VIDEO = 1,
    AUDIO = 2,
    VIDEO_AUDIO = 3
};

class FFmpegRecorder {

public:

    explicit FFmpegRecorder();

    ~FFmpegRecorder();

    int record(RecordContent recordContent);

private:

    int openDevice(RecordContent recordContent);

    int beginRecord();

    int doRecord();

    int deallocate();

    char errorMessage[100];

    AVDictionary *options = nullptr;
    AVInputFormat *inputFormat;
    AVFormatContext *formatContext;

};


#endif //J_TOOL_FFMPEGRECORDER_H
