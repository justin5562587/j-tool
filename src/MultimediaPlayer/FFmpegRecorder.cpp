//
// Created by justin on 2021/02/07.
//

#include "FFmpegRecorder.h"

// [[video device]:[audio device]]
// https://ffmpeg.org/ffmpeg-devices.html
const char* deviceName = ":0";

FFmpegRecorder::FFmpegRecorder() {}

FFmpegRecorder::~FFmpegRecorder() {}

int FFmpegRecorder::recordAudio() {
    int ret;
    ret = openDevice();
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

int FFmpegRecorder::deallocate() {
    return 0;
}

