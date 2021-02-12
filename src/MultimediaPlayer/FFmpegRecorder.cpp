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
        ret = openDevice();
        if (ret < 0) return ret;
        ret = createOutfile();
        ret = beginScale();
        if (ret < 0) return ret;
        ret = doRecord();
        if (ret < 0) return ret;
        ret = deallocate();
        if (ret < 0) return ret;
    }
    return 0;
}

// private functions

int FFmpegRecorder::openDevice(RecordContent recordContent, AVPixelFormat dstFormat, const char* outFilename) {
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

    // initialize && open codec relevant params
    videoStreamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &videoCodec, 0);
    videoCodecContext = avcodec_alloc_context3(videoCodec);
    ret = avcodec_parameters_to_context(videoCodecContext, formatContext->streams[videoStreamIndex]->codecpar);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        av_log(nullptr, AV_LOG_ERROR, "avcodec_parameters_to_context: %s", errorMessage);
    }
    ret = avcodec_open2(videoCodecContext, videoCodec, nullptr);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        av_log(nullptr, AV_LOG_ERROR, "avcodec_open2: %s", errorMessage);
    }

    // create outfile
    char const *filename = recordContent == VIDEO ? "/Users/justin/Downloads/outfile.yuv" : "/Users/justin/Downloads/outfile.pcm";

    return 0;
}

int FFmpegRecorder::scale(AVFrame *frame, AVPacket *pkt, std::ofstream *outfile) {
    int ret = 0;
    /* send the frame to the encoder */
    ret = avcodec_send_frame(videoCodecContext, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        return ret;
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(videoCodecContext, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return 0;
        } else if (ret < 0) {
            return ret;
        }
        outfile->write((char *) pkt->data, pkt->size);
        av_packet_unref(pkt);
    }
    return ret;
}

int FFmpegRecorder::beginScale() {
    int ret = 0;
    swsContext = sws_getContext(
            videoCodecContext->width,
            videoCodecContext->height,
            videoCodecContext->pix_fmt,
            videoCodecContext->width,
            videoCodecContext->height,
            AV_PIX_FMT_RGB24,
            SWS_BICUBIC,
            nullptr, nullptr, nullptr
    );

    return ret;
}

int FFmpegRecorder::doRecord(RecordContent recordContent) {
    int ret = 0;
    int times = 0;
    AVPacket *packet = av_packet_alloc();



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
//        if (recordContent == VIDEO) {
//            outfile.write((char *) packet->data, 462720);
//        } else {
//            outfile.write((char *) packet->data, packet->size);
//        }
//        outfile.flush();

        times++;
        av_packet_unref(packet);
    }

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
