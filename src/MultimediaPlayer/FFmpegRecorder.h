//
// Created by justin on 2021/02/07.
//

#ifndef J_TOOL_FFMPEGRECORDER_H
#define J_TOOL_FFMPEGRECORDER_H

#include <fstream>

extern "C" {
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
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

    int beginScale();

    int scale(AVFrame *frame, AVPacket *pkt, std::ofstream *outfile);

    int doRecord(RecordContent recordContent);

    int deallocate();

    int isRecording = -1;
    int abortSignal = -1;
    int isAllocated = -1;
    char errorMessage[100];

    int videoStreamIndex = -1;
    int audioStreamIndex = -1;

    AVDictionary *options = nullptr;
    AVInputFormat *inputFormat;
    AVFormatContext *formatContext;

    AVCodec *videoCodec;
    AVCodecContext *videoCodecContext;
    SwsContext *swsContext;

};


#endif //J_TOOL_FFMPEGRECORDER_H
