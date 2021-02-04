//
// Created by justin on 2021/02/03.
//

#ifndef J_TOOL_FFMPEGDECODER_H
#define J_TOOL_FFMPEGDECODER_H

#include <string>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
};

class FFmpegDecoder {
public:

    explicit FFmpegDecoder();

    ~FFmpegDecoder();

    int openFile(const std::string& filename);

    int openCodec();

    void beginDecode(AVPixelFormat dstFormat);

    int decode();

    void deallocate();

private:

    AVFormatContext *formatContext;
    AVCodec *videoCodec;
    AVCodecContext *videoCodecContext;
    AVCodec *audioCodec;
    AVCodecContext *audioCodecContext;
    AVStream *videoStream;
    AVStream *audioStream;

    uint8_t *buffer;
    AVFrame *frame;
    AVFrame *retFrame;
    AVPacket *packet;
    SwsContext *swsContext;

    int videoStreamIndex;
    int audioStreamIndex;


};


#endif //J_TOOL_FFMPEGDECODER_H
