//
// Created by justin on 2020/11/19.
//
#include "./MultimediaPlayer.h"

// include ffmpeg
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include <cstdio>
//#include <stdio.h>

MultimediaPlayer::MultimediaPlayer() {
    av_register_all();
}

void MultimediaPlayer::openFile() {
    AVFormatContext *pFormatCtx = NULL;

    // open video file
    if (avformat_open_input(&pFormatCtx, NULL, NULL, 0, NULL) != 0) {
        return -1;
    }

    // Retrieve stream information
    if(avformat_find_stream_info(pFormatCtx, NULL)<0)
        return -1; // Couldn't find stream information
}
