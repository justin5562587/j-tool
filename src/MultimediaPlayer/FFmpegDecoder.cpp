//
// Created by justin on 2021/02/03.
//

#include "FFmpegDecoder.h"

const std::string diskPath = "/Users/justin/Downloads";
AVPixelFormat dstFormat = AV_PIX_FMT_RGB24;
//AVPixelFormat dstFormat = AV_PIX_FMT_YUV420P;

FFmpegDecoder::FFmpegDecoder() {

}

FFmpegDecoder::~FFmpegDecoder() {

}

int FFmpegDecoder::decodeVideo(const std::string &filename) {
    int ret;
    ret = openFile(filename);
    if (ret < 0) return ret;
    ret = openCodec();
    if (ret < 0) return ret;
    ret = beginDecode();
    if (ret < 0) return ret;
    ret = decode();
    if (ret < 0) return ret;
    deallocate();

    return 0;
}


int FFmpegDecoder::openFile(const std::string &filename) {
    int ret;
    formatContext = avformat_alloc_context();
    ret = avformat_open_input(&formatContext, filename.c_str(), nullptr, nullptr);
    if (ret < 0) return ret;

    ret = avformat_find_stream_info(formatContext, nullptr);
    if (ret < 0) return ret;

    videoStreamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &videoCodec, -1);
    if (videoStreamIndex >= 0) {
        videoStream = formatContext->streams[videoStreamIndex];
    } else {
        return -1;
    }

    audioStreamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, &audioCodec, -1);

    return 0;
}

int FFmpegDecoder::openCodec() {
    int ret;
    videoCodecContext = avcodec_alloc_context3(videoCodec);
    ret = avcodec_parameters_to_context(videoCodecContext, videoStream->codecpar);
    if (ret < 0) return ret;

    ret = avcodec_open2(videoCodecContext, videoCodec, nullptr);
    if (ret != 0) return ret;

    return 0;
}

int FFmpegDecoder::beginDecode() {
    swsContext = sws_alloc_context();
    sws_getContext(
            videoCodecContext->width,
            videoCodecContext->height,
            videoCodecContext->pix_fmt,
            videoCodecContext->width,
            videoCodecContext->height,
            dstFormat,
            SWS_BICUBIC,
            nullptr, nullptr, nullptr
    );
    frame = av_frame_alloc();
    retFrame = av_frame_alloc();
    packet = av_packet_alloc();
    int nBytes = av_image_get_buffer_size(dstFormat, videoCodecContext->width, videoCodecContext->height, 32);
    buffer = (uint8_t *) av_malloc(nBytes * sizeof(uint8_t));

    av_image_fill_arrays(
            retFrame->data,
            retFrame->linesize,
            buffer,
            dstFormat,
            videoCodecContext->width,
            videoCodecContext->height,
            32
    );

    return 0;
}

int FFmpegDecoder::decode() {
    int ret;
    int times = 0;
    while (av_read_frame(formatContext, packet) == 0 && ++times != 50) {
        ret = avcodec_send_packet(videoCodecContext, packet);
        if (ret != 0) return ret;

        ret = avcodec_receive_frame(videoCodecContext, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            continue;
        } else if (ret < 0) {
            return ret;
        }

        sws_scale(
                swsContext,
                frame->data,
                frame->linesize,
                0,
                videoCodecContext->height,
                retFrame->data,
                retFrame->linesize
        );

        // todo write frame to disk image
        std::cout << retFrame->pts << std::endl;

        av_packet_unref(packet);
    }

    return 0;
}

int FFmpegDecoder::deallocate() {
    avformat_close_input(&formatContext);
    avcodec_free_context(&videoCodecContext);
    av_frame_free(&frame);
    av_frame_free(&retFrame);
    av_packet_free(&packet);
    sws_freeContext(swsContext);
    av_free(buffer);
    av_free(videoStream);
    return 0;
}

