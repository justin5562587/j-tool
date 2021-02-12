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

    // set custom value in RecordInfo
    recordInfo.recordContent = recordContent;

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
    if (recordContent == VIDEO) {
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
    } else {
        audioStreamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, &audioCodec, 0);
        audioCodecContext = avcodec_alloc_context3(audioCodec);
        ret = avcodec_parameters_to_context(audioCodecContext, formatContext->streams[audioStreamIndex]->codecpar);
        if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(nullptr, AV_LOG_ERROR, "avcodec_parameters_to_context: %s", errorMessage);
        }
        ret = avcodec_open2(audioCodecContext, videoCodec, nullptr);
        if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(nullptr, AV_LOG_ERROR, "avcodec_open2: %s", errorMessage);
        }
    }

    return 0;
}

int FFmpegRecorder::doRecord() {
    int ret = 0, times = 0, outFrameBufferSize = 0;
    AVPacket *packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    av_image_alloc(
            frame->data, frame->linesize, videoCodecContext->width, videoCodecContext->height,
            videoCodecContext->pix_fmt, 32
    );
    AVFrame *outFrame = av_frame_alloc();
    outFrameBufferSize = av_image_alloc(
            outFrame->data, outFrame->linesize, videoCodecContext->width, videoCodecContext->height,
            recordInfo.dstFormat, 32
    );
    SwsContext* swsContext = sws_getContext(
            videoCodecContext->width, videoCodecContext->height, videoCodecContext->pix_fmt,
            videoCodecContext->width, videoCodecContext->height, recordInfo.dstFormat,
            SWS_BICUBIC,
            nullptr, nullptr, nullptr
    );

    // create outfile
    std::stringstream fullFilename;
    fullFilename << recordInfo.outDiskPath << recordInfo.outFilename;
    fullFilename << (recordInfo.recordContent == VIDEO ? ".yuv" : ".pcm");
    std::ofstream outfile(fullFilename.str(), std::ios::out | std::ios::binary);

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
            break;
        }
        av_log(nullptr, AV_LOG_INFO, "packet->size: %d, packet->data: %p\n", packet->size, packet->data);
//        if (recordInfo.recordContent == VIDEO) {
//            outfile.write((char *) outFrame->data, 462720);
//        } else {
//            outfile.write((char *) outFrame->data, outFrame->linesize);
//        }

        ret = avcodec_send_packet(videoCodecContext, packet);
        if (ret != 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(nullptr, AV_LOG_ERROR, "av_read_frame: %s", errorMessage);
            return ret;
        }

        ret = avcodec_receive_frame(videoCodecContext, frame);
        if (ret == AVERROR(EAGAIN)) {
            continue;
        } else if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(nullptr, AV_LOG_ERROR, "avcodec_receive_frame: %s", errorMessage);
            break;
        }

        sws_scale(
                swsContext,
                (const uint8_t *const *) frame->data, frame->linesize, 0, videoCodecContext->height,
                outFrame->data, outFrame->linesize
        );

        if (recordInfo.recordContent == VIDEO) {
            outfile.write((char * ) outFrame->data[0], outFrameBufferSize);
        } else {
//            outfile.write((char *) outFrame->data, outFrame->linesize);
        }

        times++;
        av_packet_unref(packet);
    }

    outfile.close();
    sws_freeContext(swsContext);
    av_packet_free(&packet);
    av_frame_free(&frame);
    av_frame_free(&outFrame);
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
