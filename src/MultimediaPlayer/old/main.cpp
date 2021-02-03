//
// Created by justin on 2021/01/19.
//

#include <string>
#include <fstream>
#include <iostream>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
}

// 功能->打开电脑摄像头，并且av_read_frame 500次，log出每次packet的size
int main () {
    avdevice_register_all();

    int ret = 0;
    char errorMessage[1024];
    // 0 -> 使用摄像头录制
    // 1- > 录制桌面
    char *deviceName = "0";
    AVFormatContext *pFormatCtx = nullptr;
    AVDictionary *options = nullptr;

    av_dict_set(&options, "video_size", "640*480", 0);
    av_dict_set(&options, "framerate", "30", 0);
    av_dict_set(&options, "pixel_format", "uyvy422", 0);

    // get format
    AVInputFormat *pInputFormat = av_find_input_format("avfoundation");
    if (pInputFormat == nullptr) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        std::cout << "avformat_open_input: " << errorMessage << std::endl;
        return ret;
    }

    ret = avformat_open_input(&pFormatCtx, deviceName, pInputFormat, &options);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        std::cout << "avformat_open_input: " << errorMessage << std::endl;
        return ret;
    }

    int count = 0;
    AVPacket packet;
    av_init_packet(&packet);

    while ((ret = av_read_frame(pFormatCtx, &packet)) == 0 && count < 500) {
        std::cout << "packet.size: " << packet.size << std::endl;
        av_packet_unref(&packet);
        ++count;
    }
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        std::cout << "av_read_frame: " << errorMessage << std::endl;
        return ret;
    }

    avformat_close_input(&pFormatCtx);
    av_log(nullptr, AV_LOG_DEBUG, "Record Video Finished.\n");

    return ret;
}
