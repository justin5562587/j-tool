//
// Created by justin on 2021/01/19.
//

#pragma once

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

#include "./AdvancedControl.h"

int initializeFFmpeg(const std::string &filepath);

/**
 * initialize avformatContext to record audio data
 */
int recordAudioWithFFmpeg(AdvancedControl *advancedControl, const std::string &diskPath);

/**
 * initialize avformatContext to record video data
 */
int recordVideoWithFFmpeg(AdvancedControl *advancedControl, const std::string &diskPath);

/**
 * write audio data to disk with pcm extension
 */
void writeAudioDataToDisk(const std::string &diskPath, AVPacket *packet);

/**
 * write video data to disk with specific extension
 */
void writeVideoDataToDisk(const std::string &diskPath, AVPacket *packet);
