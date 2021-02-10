//
// Created by justin on 2021/02/07.
//

#include "FFmpegRecorder.h"

// [[video device]:[audio device]]
// https://ffmpeg.org/ffmpeg-devices.html
const char *audioDeviceName = ":0";
const char *videoDeviceName = "0";

FFmpegRecorder::FFmpegRecorder() {
    avdevice_register_all();
    av_log_set_level(AV_LOG_DEBUG);
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

    if (recordContent == VIDEO) {
        deviceName = videoDeviceName;
        av_dict_set(&options, "video_size", "640*480", 0);
        av_dict_set(&options, "framerate", "30", 0);
        av_dict_set(&options, "pixel_format", "nv12", 0);
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
    int ret = 0;
    int times = 0;
    AVPacket *packet = av_packet_alloc();

    // create file
    const char *filename = "/Users/justin/Downloads/outfile.yuv";
//    std::fstream outfile;
//    outfile.open("", std::ios::out);
    FILE *outfile = fopen(filename, "wb+");

    while (true) {
        if (times >= 500 || abortSignal == 1) {
            deallocate();
            break;
        }

        ret = av_read_frame(formatContext, packet);
        if (ret == AVERROR(EAGAIN)) {
            continue; // if av_read_frame return -35 continue
        } else if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(nullptr, AV_LOG_ERROR, "av_read_frame: %s", errorMessage);
            av_packet_free(&packet);
            break;
        }

        av_log(nullptr, AV_LOG_INFO, "packet->size: %d, packet->size: %p\n", packet->size, packet->data);
//        outfile.write((char *) packet->data, packet->size);
//        fwrite(packet->data, packet->size, 1, outfile); // for audio
        fwrite(packet->data, 1, 61440, outfile); // for video
        fflush(outfile);

        times++;

        av_packet_unref(packet);
    }

    fclose(outfile);

//    outfile.close();
    return ret;
}

int FFmpegRecorder::deallocate() {
    if (isAllocated != 1) {
        avformat_close_input(&formatContext);
        options = nullptr;
    }
    isAllocated = 1;
    return 0;
}
