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
    RecordContent recordContent;
    AVPixelFormat outFormat;
    const char *outDiskPath;
    const char *outFilename;
    const char *outFileExtension;
} RecordInfo;

class FFmpegRecorder {

public:

    explicit FFmpegRecorder();

    ~FFmpegRecorder();

    // delete original copy/assign
    FFmpegRecorder(const FFmpegRecorder&) = delete;
    FFmpegRecorder* operator=(const FFmpegRecorder&) = delete;

    // define move copy/assign
    FFmpegRecorder(FFmpegRecorder &&ffmpegRecorder);
    FFmpegRecorder& operator=(FFmpegRecorder &&ffmpegRecorder) noexcept;

    void startRecord(RecordContent recordContent);

    void stopRecord();

    int record(RecordContent recordContent);

//protected:
//    void run();

private:

    int initializeInputDevice(RecordContent recordContent);

    int initializeOutfile();

    int decodeInVideo(SwsContext *swsContext, AVPacket *inPacket, AVFrame *frame, AVFrame *yuvFrame);

    int encodeOutVideo(AVFrame *yuvFrame);

    int doRecord();

    int deallocate();

    RecordInfo recordInfo {
        VIDEO,
        AV_PIX_FMT_YUV420P,
        "/Users/justin/Downloads/",
        "outfile",
        ".mp4"
    };

    std::thread recordVideoThread;
    std::thread encodeThread;
    std::thread testThread;

    int isRecording = -1;
    int abortSignal = -1;
    int isAllocated = -1;
    char errorMessage[100];

    // fields for input
    AVDictionary *options = nullptr;
    AVInputFormat *inputFormat;
    AVFormatContext *inputFormatContext;
    int inVStreamIndex = -1;
    AVCodec *inVCodec;
    AVCodecContext *inVCodecContext;
    int inAStreamIndex = -1;
    AVCodec *inACodec;
    AVCodecContext *inACodecContext;

    // fields for output
    AVFormatContext *outputFormatContext;
    AVCodec *outVCodec;
    AVCodecContext *outVCodecContext;
    AVStream *outVStream;

};

#endif //J_TOOL_FFMPEGRECORDER_H
