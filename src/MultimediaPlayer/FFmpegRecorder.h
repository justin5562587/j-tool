//
// Created by justin on 2021/02/07.
//

#ifndef J_TOOL_FFMPEGRECORDER_H
#define J_TOOL_FFMPEGRECORDER_H

#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <iostream>

extern "C" {
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/timestamp.h>
};

enum RecordContent {
    VIDEO = 1,
    AUDIO = 2,
    VIDEO_AUDIO = 3
};

typedef struct RecordInfo {
    int fps;
    RecordContent recordContent;
    AVPixelFormat outFormat;
    const char *outDiskPath;
    const char *outFilename;
    const char *outFileExtension;
} RecordInfo;

//https://stackoverflow.com/questions/10673585/start-thread-with-member-function
class FFmpegRecorder {

public:

    explicit FFmpegRecorder();

    ~FFmpegRecorder();

    // delete original copy/assign
    FFmpegRecorder(const FFmpegRecorder&) = delete;
    FFmpegRecorder* operator=(const FFmpegRecorder&) = delete;

    // define assign-copy using std::move
    FFmpegRecorder& operator=(FFmpegRecorder &&ffmpegRecorder) noexcept;

    void doRecord(RecordContent recordContent);

private:

    int record(RecordContent recordContent);

    void registerRecordInfo(RecordContent recordContent);

    int initializeInputDevice();

    int initializeOutputFile();

    int processRecord();

    void deallocate();

    int decodeInVideo(SwsContext *swsContext, AVPacket *inPacket, AVFrame *frame, AVFrame *yuvFrame);

    int encodeOutVideo(AVFrame *yuvFrame);

    RecordInfo recordInfo {
        30,
        VIDEO,
        AV_PIX_FMT_YUV420P,
        "/Users/justin/Downloads/",
        "outfile",
        ".mp4"
    };

    std::thread recordVideoThread;

    int abortSignal = -1;
    int isRunning = -1;
    int isAllocated = -1;

    char errorMessage[100];

    // fields for input
    AVDictionary *options = nullptr;
    AVFormatContext *inputFormatContext;
    int inVStreamIndex = -1;
    AVCodecContext *inVCodecContext;
    int inAStreamIndex = -1;
    AVCodecContext *inACodecContext;

    // fields for output
    AVFormatContext *outputFormatContext;
    AVCodecContext *outVCodecContext;
    AVStream *outVStream;

};

#endif //J_TOOL_FFMPEGRECORDER_H
