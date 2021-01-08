//
// Created by justin on 2021/01/08.
//

#include <string>
#include <QDebug>

extern "C" {

#include "./frameProcessor.h"
#include <libavformat/avformat.h>

int logInfoAboutFile(const std::string &fileUrl) {
//int logInfoAboutFile(const char* url) {
    AVFormatContext *pFormatCtx;

    const char *url = fileUrl.c_str();
    int ret = avformat_open_input(&pFormatCtx, url, nullptr, nullptr);
    if (ret < 0) {
        qInfo() << "Open file error";
        return -1;
    }

    ret = avformat_find_stream_info(pFormatCtx, nullptr);
    if (ret < 0) {
        qInfo() << "Cannot find stream info";
        return -1;
    }

    qInfo() << pFormatCtx->duration;
    qInfo() << pFormatCtx->bit_rate;
    qInfo() << pFormatCtx->nb_streams;

    return 0;
}

}
