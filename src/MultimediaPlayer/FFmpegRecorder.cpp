//
// Created by justin on 2021/02/07.
//

#include "FFmpegRecorder.h"

// [[video device]:[audio device]]
// https://ffmpeg.org/ffmpeg-devices.html
const char *deviceName = ":0";

FFmpegRecorder::FFmpegRecorder() {
    av_log_set_level(AV_LOG_INFO);
}

FFmpegRecorder::~FFmpegRecorder() {}

int FFmpegRecorder::recordAudio() {
    int ret;
    ret = openDevice();
    if (ret < 0) return ret;
    ret = beginRecord();
    if (ret < 0) return ret;
    ret = record();
    if (ret < 0) return ret;
    ret = deallocate();
    if (ret < 0) return ret;

    return 0;
}

int FFmpegRecorder::openDevice() {
    int ret;
    avdevice_register_all();

    inputFormat = av_find_input_format("avfoundation");

    formatContext = avformat_alloc_context();
    ret = avformat_open_input(&formatContext, deviceName, inputFormat, nullptr);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        av_log(nullptr, AV_LOG_ERROR, "Open device error: %s", errorMessage);
    }

    return 0;
}

int FFmpegRecorder::beginRecord() {

    return 0;
}

int FFmpegRecorder::record() {
    int ret, count = 0;
    AVPacket *packet = av_packet_alloc();

    // todo av_read_frame will return -35
    while ((ret = av_read_frame(formatContext, packet)) == 0 && ++count < 50) {
        av_log(nullptr, AV_LOG_INFO, "packet->size: %d", packet->size);
        av_packet_unref(packet);
    }
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        av_log(nullptr, AV_LOG_ERROR, "av_read_frame: %s", errorMessage);
    }

    av_packet_free(&packet);
    return 0;
}

int FFmpegRecorder::deallocate() {
    avformat_close_input(&formatContext);
    return 0;
}

