//
// Created by justin on 2021/02/07.
//

#ifndef J_TOOL_FFMPEGRECORDER_H
#define J_TOOL_FFMPEGRECORDER_H

#include <fstream>
#include <string>

extern "C" {
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
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
    char *outFilename;
} RecordInfo;

class FFmpegRecorder {

public:

    explicit FFmpegRecorder();

    ~FFmpegRecorder();

    int record(RecordContent recordContent);

private:

    int openDevice(RecordContent recordContent, AVPixelFormat dstFormat, const char* outFilename);

    int createOutfile();

    int beginScale();

    int scale(AVFrame *frame, AVPacket *pkt, std::ofstream *outfile);

    int doRecord();

    int deallocate();

    RecordInfo *recordInfo;

    // fields below are for FFmpeg

    std::ofstream *outfile;

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
    SwsContext *swsContext;
    int audioStreamIndex = -1;
    AVCodec *audioCodec;
    AVCodecContext *audioCodecContext;
    SwrContext *swrContext;

};

#endif //J_TOOL_FFMPEGRECORDER_H
