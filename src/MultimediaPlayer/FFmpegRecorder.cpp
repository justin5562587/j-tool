//
// Created by justin on 2021/02/07.
//

#include "FFmpegRecorder.h"

// [[video device]:[audio device]]
// https://ffmpeg.org/ffmpeg-devices.html
const char *audioDeviceName = ":0";
const char *videoDeviceName = "0";

FFmpegRecorder::FFmpegRecorder() {
    av_log_set_level(AV_LOG_INFO);
}

FFmpegRecorder::~FFmpegRecorder() {}

// public functions

int FFmpegRecorder::record(RecordContent recordContent) {
    if (isRecording == 1) {
        abortSignal = 1;
    } else {
        abortSignal = -1;
        isAllocated = -1;
        isRecording = 1;
        int ret;
        ret = openDevice(recordContent);
        if (ret < 0) return ret;
        ret = beginRecord();
        if (ret < 0) return ret;
        ret = doRecord();
        if (ret < 0) return ret;
        ret = deallocate();
        if (ret < 0) return ret;
    }
    return 0;
}

// private functions

int FFmpegRecorder::openDevice(RecordContent recordContent) {
    int ret;
    const char *deviceName;
    avdevice_register_all();

    if (recordContent == VIDEO) {
        deviceName = videoDeviceName;
        av_dict_set(&options, "video_size", "640*480", 0);
        av_dict_set(&options, "framerate", "30", 0);
    } else if (recordContent == AUDIO) {
        deviceName = audioDeviceName;
    }

    inputFormat = av_find_input_format("avfoundation");

    formatContext = avformat_alloc_context();
    ret = avformat_open_input(&formatContext, deviceName, inputFormat, &options);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        av_log(nullptr, AV_LOG_ERROR, "Open device error: %s", errorMessage);
    }

    return 0;
}

int FFmpegRecorder::beginRecord() {
    return 0;
}

int FFmpegRecorder::doRecord() {
    int ret;
    AVPacket *packet = av_packet_alloc();

    // create file
    std::fstream outfile;
    outfile.open("/Users/justin/Downloads/outfile.yuv", std::ios::out);

    while (true) {
        if (abortSignal == 1) {
            deallocate();
            break;
        }

        ret = av_read_frame(formatContext, packet);
        if (ret < 0) {
            if (ret == AVERROR(EAGAIN)) {
                continue; // if av_read_frame return -35 continue
            } else {
                av_strerror(ret, errorMessage, sizeof(errorMessage));
                av_log(nullptr, AV_LOG_ERROR, "av_read_frame: %s", errorMessage);
                av_packet_free(&packet);
                return ret;
            }
        }
        av_log(nullptr, AV_LOG_INFO, "packet->size: %d, packet->size: %p\n", packet->size, packet->data);
        outfile.write((char *) packet->data, packet->size);

        av_packet_unref(packet);
    }

    outfile.close();
    return 0;
}

int FFmpegRecorder::deallocate() {
    if (isAllocated != 1) {
        avformat_close_input(&formatContext);
        options = nullptr;
    }
    isAllocated = 1;
    return 0;
}
