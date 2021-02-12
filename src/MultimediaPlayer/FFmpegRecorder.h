//
// Created by justin on 2021/02/07.
//

#ifndef J_TOOL_FFMPEGRECORDER_H
#define J_TOOL_FFMPEGRECORDER_H

#include <fstream>
#include <string>
#include <sstream>

extern "C" {
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
};

enum RecordContent {
    VIDEO = 1,
    AUDIO = 2,
    VIDEO_AUDIO = 3
};

typedef struct RecordInfo {
    RecordContent recordContent;
    AVPixelFormat dstFormat;
    const char *outDiskPath;
    const char *outFilename;
} RecordInfo;

class FFmpegRecorder {

public:

    explicit FFmpegRecorder();

    ~FFmpegRecorder();

    int record(RecordContent recordContent);

private:

    int initializeInputDevice(RecordContent recordContent);

    int initializeOutfile();

    int doRecord();

    int deallocate();

    RecordInfo recordInfo {
        VIDEO,
        AV_PIX_FMT_RGB24,
        "/Users/justin/Downloads/",
        "outfile",
    };

    // fields below are for FFmpeg
    int isRecording = -1;
    int abortSignal = -1;
    int isAllocated = -1;
    char errorMessage[100];

    AVDictionary *options = nullptr;
    AVInputFormat *inputFormat;
    AVFormatContext *formatContext;

    int videoStreamIndex = -1;
    AVCodec *videoCodec;
    AVCodecContext *videoCodecContext;
//    SwsContext *swsContext;
    int audioStreamIndex = -1;
    AVCodec *audioCodec;
    AVCodecContext *audioCodecContext;
    SwrContext *swrContext;

};

#endif //J_TOOL_FFMPEGRECORDER_H
