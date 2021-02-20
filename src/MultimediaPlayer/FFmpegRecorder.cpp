//
// Created by justin on 2021/02/07.
//

#include "FFmpegRecorder.h"

// [[video device]:[audio device]]
// https://ffmpeg.org/ffmpeg-devices.html
#define AUDIO_DEVICE_NAME ":0"
#define VIDEO_DEVICE_NAME "0"
#define DEVICE_NAME "avfoundation"

void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt, const char *tag) {
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;
    printf("%s: pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           tag,
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);
}

//void FFmpegRecorder::run() {
//
//}

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
        ret = initializeInputDevice(recordContent);
        if (ret < 0) return ret;
        ret = initializeOutfile();
        if (ret < 0) return ret;
        ret = doRecord();
        if (ret < 0) return ret;
        ret = deallocate();
        if (ret < 0) return ret;
    }
    return 0;
}

// private functions
int FFmpegRecorder::initializeInputDevice(RecordContent recordContent) {
    int ret;
    const char *deviceName;

    // set custom value in RecordInfo
    recordInfo.recordContent = recordContent;

    if (recordContent == VIDEO) {
        deviceName = VIDEO_DEVICE_NAME;
        av_dict_set(&options, "video_size", "640*480", 0);
        av_dict_set(&options, "framerate", "30", 0);
        av_dict_set(&options, "pixel_format", "nv12", 0);
    } else if (recordContent == AUDIO) {
        deviceName = AUDIO_DEVICE_NAME;
    }

    inputFormat = av_find_input_format(DEVICE_NAME);
    inputFormatContext = avformat_alloc_context();
    ret = avformat_open_input(&inputFormatContext, deviceName, inputFormat, &options);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        av_log(nullptr, AV_LOG_ERROR, "Open device error: %s", errorMessage);
    }

    // initialize && open codec relevant params
    if (recordContent == VIDEO) {
        inVStreamIndex = av_find_best_stream(inputFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &inVCodec, 0);
        inVCodecContext = avcodec_alloc_context3(inVCodec);

        ret = avcodec_parameters_to_context(inVCodecContext, inputFormatContext->streams[inVStreamIndex]->codecpar);
        if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(nullptr, AV_LOG_ERROR, "avcodec_parameters_to_context: %s", errorMessage);
        }
        ret = avcodec_open2(inVCodecContext, inVCodec, nullptr);
        if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(nullptr, AV_LOG_ERROR, "avcodec_open2: %s", errorMessage);
        }
    } else {
        inAStreamIndex = av_find_best_stream(inputFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, &inACodec, 0);
        inACodecContext = avcodec_alloc_context3(inACodec);
        ret = avcodec_parameters_to_context(inACodecContext, inputFormatContext->streams[inAStreamIndex]->codecpar);
        if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(nullptr, AV_LOG_ERROR, "avcodec_parameters_to_context: %s", errorMessage);
        }
        ret = avcodec_open2(inACodecContext, inACodec, nullptr);
        if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(nullptr, AV_LOG_ERROR, "avcodec_open2: %s", errorMessage);
        }
    }

    av_dump_format(inputFormatContext, inVStreamIndex, nullptr, 0);
    return 0;
}

// https://stackoverflow.com/questions/46444474/c-ffmpeg-create-mp4-file
int FFmpegRecorder::initializeOutfile() {
    int ret = 0;
    char fullFilename[100];
    strcat(fullFilename, recordInfo.outDiskPath);
    strcat(fullFilename, recordInfo.outFilename);
    strcat(fullFilename, recordInfo.outFileExtension);

    outputFormat = av_guess_format(nullptr, fullFilename, nullptr);
    outputFormatContext = avformat_alloc_context();
    avformat_alloc_output_context2(&outputFormatContext, outputFormat, nullptr, fullFilename);

    outVCodec = avcodec_find_encoder(outputFormat->video_codec);
    outVStream = avformat_new_stream(outputFormatContext, outVCodec);
    outVCodecContext = avcodec_alloc_context3(outVCodec);

    outVStream->codecpar->codec_id = outputFormat->video_codec;
    outVStream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    outVStream->codecpar->width = inVCodecContext->width;
    outVStream->codecpar->height = inVCodecContext->height;
    outVStream->codecpar->format = AV_PIX_FMT_YUV420P; // AV_PIX_FMT_YUV420P;
    outVStream->codecpar->bit_rate = 2000 * 1000; // bitrate * 1000;
    avcodec_parameters_to_context(outVCodecContext, outVStream->codecpar);

    outVCodecContext->time_base = AVRational{1, 30};// inVCodecContext->time_base;
    outVCodecContext->max_b_frames = inVCodecContext->max_b_frames;
    outVCodecContext->gop_size = inVCodecContext->gop_size;
    outVCodecContext->framerate = AVRational{30, 1};// inVCodecContext->framerate;

    //must remove the following
    //outVCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    if (outVStream->codecpar->codec_id == AV_CODEC_ID_H264) {
        av_opt_set(outVCodecContext, "preset", "ultrafast", 0);
    }

    avcodec_parameters_from_context(outVStream->codecpar, outVCodecContext);

    avcodec_open2(outVCodecContext, outVCodec, nullptr);

    if (!(outputFormat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&outputFormatContext->pb, fullFilename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(outputFormatContext, AV_LOG_ERROR, "avio_open: %s", errorMessage);
            return ret;
        }
    }

    // avformat_write_header will overwrite AVStream.time_base according to file extension
    // AVRational{1, 90000} if mpeg
    ret = avformat_write_header(outputFormatContext, nullptr);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        av_log(outputFormatContext, AV_LOG_ERROR, "avformat_write_header: %s", errorMessage);
        return ret;
    }

    av_dump_format(outputFormatContext, 0, fullFilename, 1);
    return 0;
}

int FFmpegRecorder::decodeSourceVideo(SwsContext *swsContext, AVPacket *packet, AVFrame *frame, AVFrame *yuvFrame) {
    int ret = 0;

    ret = avcodec_send_packet(inVCodecContext, packet);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        av_log(inVCodecContext, AV_LOG_ERROR, "avcodec_send_packet: %s", errorMessage);
        return ret;
    }

    ret = avcodec_receive_frame(inVCodecContext, frame);
    if (ret == AVERROR(EAGAIN)) {
        return 0; // non-enough data, return 0 for continue
    } else if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        av_log(inVCodecContext, AV_LOG_ERROR, "avcodec_receive_frame: %s", errorMessage);
        return ret;
    }

    sws_scale(
            swsContext,
            (uint8_t const *const *) frame->data,
            frame->linesize,
            0,
            inVCodecContext->height,
            yuvFrame->data,
            yuvFrame->linesize
    );

    yuvFrame->width = inVCodecContext->width;
    yuvFrame->height = inVCodecContext->height;
    yuvFrame->format = AV_PIX_FMT_YUV420P;
    return ret;
}

int FFmpegRecorder::encodeOutVideo(AVFrame *yuvFrame, AVPacket *outPacket) {
    int ret = 0;

    ret = avcodec_send_frame(outVCodecContext, yuvFrame);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        av_log(nullptr, AV_LOG_ERROR, "avcodec_send_frame: %s", errorMessage);
        return ret;
    }

    ret = avcodec_receive_packet(outVCodecContext, outPacket);
    if (ret == AVERROR(EAGAIN)) {
        return 0; // non-enough data, return 0 for continue;
    } else if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        av_log(nullptr, AV_LOG_ERROR, "avcodec_receive_packet: %s", errorMessage);
        return ret;
    }

    return ret;
}


int FFmpegRecorder::doRecord() {
    int ret = 0, times = 0;
    int ptsBase = 0;
    int packetCount = 0;
    AVPacket packet;
    AVPacket *outPacket = av_packet_alloc();
//    av_init_packet(outPacket);
//    AVStream *inStream, *outStream;

    SwsContext *swsContext = sws_getContext(
            inVCodecContext->width,
            inVCodecContext->height,
            inVCodecContext->pix_fmt,
            inVCodecContext->width,
            inVCodecContext->height,
            AV_PIX_FMT_YUV420P,
            SWS_BICUBIC,
            nullptr, nullptr, nullptr
    );

    AVFrame *frame = av_frame_alloc();
    AVFrame *yuvFrame = av_frame_alloc();
    av_image_alloc(yuvFrame->data, yuvFrame->linesize, inVCodecContext->width, inVCodecContext->height, AV_PIX_FMT_YUV420P, 32);

    while (abortSignal != 1) {
        if (times >= 100) break;

        ret = av_read_frame(inputFormatContext, &packet);
        if (ret == AVERROR(EAGAIN)) {
            continue;
        } else if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(nullptr, AV_LOG_ERROR, "av_read_frame: %s", errorMessage);
            break;
        }

        av_log(nullptr, AV_LOG_FATAL, "INPUT packet: pts->%lld, size->%d, data->%p\n", packet.pts, packet.size, packet.data);

        if (packet.stream_index == inVStreamIndex) {
            ret = decodeSourceVideo(swsContext, &packet, frame, yuvFrame);
            if (ret < 0) return ret;
            yuvFrame->pts = ptsBase++;

            ret = encodeOutVideo(yuvFrame, outPacket);
            if (ret < 0) return ret;

//            // copy packet
//            outPacket.pts = av_rescale_q_rnd(outPacket.pts, inStream->time_base, outStream->time_base,static_cast<AVRounding>(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
//            outPacket.dts = av_rescale_q_rnd(outPacket.dts, inStream->time_base, outStream->time_base,static_cast<AVRounding>(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
//            outPacket.duration = av_rescale_q(outPacket.duration, inStream->time_base, outStream->time_base);
//            outPacket.pos = -1;

            av_log(nullptr, AV_LOG_FATAL, "OUTPUT packet: pts->%lld, size->%d, data->%p\n", outPacket->pts, outPacket->size, outPacket->data);

            av_interleaved_write_frame(outputFormatContext, outPacket);

            packetCount++;
            times++;
            av_packet_unref(&packet);
            av_packet_unref(outPacket);
        } else if (packet.stream_index == inAStreamIndex) {
            // todo
        }
    }

    av_write_trailer(outputFormatContext);
    if (!(outputFormat->flags & AVFMT_NOFILE)) {
        ret = avio_close(outputFormatContext->pb);
        if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(outputFormatContext, AV_LOG_ERROR, "avio_close: %s", errorMessage);
            return ret;
        }
    }

    av_frame_free(&frame);
    av_frame_free(&yuvFrame);
//    av_freep(yuvFrame);
    sws_freeContext(swsContext);

    return ret;
}

int FFmpegRecorder::deallocate() {
    if (isAllocated != 1) {
        avformat_close_input(&inputFormatContext);
        avformat_free_context(outputFormatContext);
        avcodec_close(inVCodecContext);
        avcodec_close(outVCodecContext);
        options = nullptr;
    }
    isAllocated = 1;
    return 0;
}
