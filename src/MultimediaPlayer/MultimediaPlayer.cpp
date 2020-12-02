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
    AVFormatContext* pFormatCtx = NULL;

    // open video file
    if (avformat_find_stream_info(&pFormatCtx, NULL, NULL, 0, NULL) != 0) {

    }
}
