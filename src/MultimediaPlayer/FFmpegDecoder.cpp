//
// Created by justin on 2021/02/03.
//

#include "FFmpegDecoder.h"

FFmpegDecoder::FFmpegDecoder() {

}

FFmpegDecoder::~FFmpegDecoder() {

}

int FFmpegDecoder::openFile(const std::string &filename) {
    int ret;
    formatContext = avformat_alloc_context();
    ret = avformat_open_input(&formatContext, filename.c_str(), nullptr, nullptr);
    if (ret < 0) return ret;

    ret = avformat_find_stream_info(formatContext, nullptr);
    if (ret < 0) return ret;

    videoStreamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &videoCodec, -1);
    if (videoStreamIndex >= 0) {
        videoStream = formatContext->streams[videoStreamIndex];
    } else {
        return -1;
    }

    audioStreamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, &audioCodec, -1);

    return 0;
}

int FFmpegDecoder::openCodec() {
    videoCodecContext = avcodec_alloc_context3(videoCodec);
    avcodec_parameters_to_context(videoCodecContext, videoStream->codecpar);
    avcodec_open2(videoCodecContext, videoCodec, nullptr);

    return 0;
}

int FFmpegDecoder::decode() {

}

