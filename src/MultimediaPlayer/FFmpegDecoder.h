//
// Created by justin on 2021/02/03.
//

#ifndef J_TOOL_FFMPEGDECODER_H
#define J_TOOL_FFMPEGDECODER_H

#include <string>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
};

class FFmpegDecoder {
public:

    explicit FFmpegDecoder();

    ~FFmpegDecoder();

    int openFile(const std::string& filename);

    int openCodec();

    int decode();

private:

    AVFormatContext *formatContext;
    AVCodec *videoCodec;
    AVCodecContext *videoCodecContext;
    AVCodec *audioCodec;
    AVCodecContext *audioCodecContext;
    AVStream *videoStream;
    AVStream *audioStream;

    int videoStreamIndex;
    int audioStreamIndex;


};


#endif //J_TOOL_FFMPEGDECODER_H
