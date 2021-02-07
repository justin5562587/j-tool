//
// Created by justin on 2021/02/03.
//

#ifndef J_TOOL_FFMPEGDECODER_H
#define J_TOOL_FFMPEGDECODER_H

#include <string>
#include <iostream>
#include <fstream>
#include <QLabel>
#include <QImage>
#include <QTime>
#include <QCoreApplication>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>
};

class FFmpegDecoder {
public:

    explicit FFmpegDecoder();

    ~FFmpegDecoder();

    int decodeMultimediaFile(const std::string& filename);

    void setScreen(QLabel *label);

    int stopDecode();

private:

    int openFile(const std::string& filename);

    int openCodec();

    int beginDecode();

    int decode();

    int decodeVideo();

    int decodeAudio(std::ofstream *outfile);

    int deallocate();

    char errorMessage[100];

    AVFormatContext *formatContext;
    AVCodec *videoCodec;
    AVCodecContext *videoCodecContext;
    AVCodec *audioCodec;
    AVCodecContext *audioCodecContext;

    uint8_t *buffer;
    AVFrame *frame;
    AVFrame *retFrame;
    AVPacket *packet;
    SwsContext *swsContext;

    int videoStreamIndex;
    int audioStreamIndex;

    QLabel *screen;

    int abortSignal = -1;
    int hasDeallocated = -1;
};

#endif //J_TOOL_FFMPEGDECODER_H
