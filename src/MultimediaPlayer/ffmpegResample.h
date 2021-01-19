//
// Created by justin on 2021/01/18.
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

int recordAudioWithFFmpeg(AdvancedControl *advancedControl);

void writeDataToDisk(const std::string &diskPath, AVPacket *packet);
