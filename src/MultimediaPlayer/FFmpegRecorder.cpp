//
// Created by justin on 2021/02/07.
//

#include "FFmpegRecorder.h"

// [[video device]:[audio device]]
// https://ffmpeg.org/ffmpeg-devices.html
#define AUDIO_DEVICE_NAME ":0"
#define VIDEO_DEVICE_NAME "0"
#define DEVICE_NAME "avfoundation"

// https://t.co/wdstsGDDi6?amp=1

void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt, const char *tag) {
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;
    printf("%s: pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           tag,
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);
}

FFmpegRecorder::FFmpegRecorder() {
    avdevice_register_all();
    av_log_set_level(AV_LOG_ERROR);
}

FFmpegRecorder::~FFmpegRecorder() {
}

FFmpegRecorder& FFmpegRecorder::operator=(FFmpegRecorder &&ffmpegRecorder) noexcept {
    this->recordVideoThread = std::move(ffmpegRecorder.recordVideoThread);
    return *this;
}

// public functions
void FFmpegRecorder::doRecord(RecordContent recordContent) {
    if (isRunning == 1) {
        abortSignal = 1;
        if (recordVideoThread.joinable()) {
            recordVideoThread.join();
        }
    } else {
        recordVideoThread = std::thread(&FFmpegRecorder::record, this, recordContent);
    }
}

// record will be executed in recordVideoThread
int FFmpegRecorder::record(RecordContent recordContent) {
    int ret;
    abortSignal = -1;
    isRunning = 1;
    registerRecordInfo(recordContent);
    ret = initializeInputDevice();
    if (ret < 0) return ret;
    ret = initializeOutputFile();
    if (ret < 0) return ret;
    ret = processRecord();
    if (ret < 0) return ret;
    deallocate();

    return 0;
}

// private functions
void FFmpegRecorder::registerRecordInfo(RecordContent recordContent) {
    recordInfo.recordContent = recordContent;
}

int FFmpegRecorder::initializeInputDevice() {
    int ret;
    const char *deviceName;

    if (recordInfo.recordContent == VIDEO) {
        deviceName = VIDEO_DEVICE_NAME;
        av_dict_set(&options, "video_size", "640*480", 0);
        av_dict_set(&options, "framerate", "30", 0);
        av_dict_set(&options, "pixel_format", "nv12", 0);
    } else if (recordInfo.recordContent == AUDIO) {
        deviceName = AUDIO_DEVICE_NAME;
    }

    AVInputFormat *inputFormat = av_find_input_format(DEVICE_NAME);
    inputFormatContext = avformat_alloc_context();
    ret = avformat_open_input(&inputFormatContext, deviceName, inputFormat, &options);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        av_log(nullptr, AV_LOG_ERROR, "Open device error: %s", errorMessage);
    }

    // initialize && open codec relevant params
    if (recordInfo.recordContent == VIDEO) {
        AVCodec *inVCodec = nullptr;
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
        AVCodec *inACodec = nullptr;
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

int FFmpegRecorder::initializeOutputFile() {
    int ret;
    char fullFilename[100];
    strcat(fullFilename, recordInfo.outDiskPath);
    strcat(fullFilename, recordInfo.outFilename);
    strcat(fullFilename, recordInfo.outFileExtension);

    outputFormatContext = avformat_alloc_context();
    avformat_alloc_output_context2(&outputFormatContext, nullptr, nullptr, fullFilename);

    AVCodec *outVCodec = avcodec_find_encoder(outputFormatContext->oformat->video_codec);
    outVStream = avformat_new_stream(outputFormatContext, outVCodec);
    outVCodecContext = avcodec_alloc_context3(outVCodec);

    outVStream->codecpar->codec_id = outputFormatContext->oformat->video_codec;
    outVStream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    outVStream->codecpar->width = inVCodecContext->width;
    outVStream->codecpar->height = inVCodecContext->height;
    outVStream->codecpar->format = AV_PIX_FMT_YUV420P;
    outVStream->codecpar->bit_rate = 2000 * 1000; // todo
    avcodec_parameters_to_context(outVCodecContext, outVStream->codecpar);

    outVCodecContext->time_base = AVRational{1, recordInfo.fps};// inVCodecContext->time_base;
    outVCodecContext->max_b_frames = inVCodecContext->max_b_frames;
    outVCodecContext->gop_size = inVCodecContext->gop_size;
    outVCodecContext->framerate = AVRational{recordInfo.fps, 1};// inVCodecContext->framerate;

    //must remove the following
    //outVCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    if (outVStream->codecpar->codec_id == AV_CODEC_ID_H264) {
        av_opt_set(outVCodecContext, "preset", "ultrafast", 0);
    }

    avcodec_parameters_from_context(outVStream->codecpar, outVCodecContext);

    avcodec_open2(outVCodecContext, outVCodec, nullptr);

    if (!(outputFormatContext->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&outputFormatContext->pb, fullFilename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(outputFormatContext, AV_LOG_ERROR, "avio_open: %s", errorMessage);
            return ret;
        }
    }

    // avformat_write_header will overwrite AVStream.time_base according to file extension
    // AVRational{1, 90000} if MPEG format, as sample rate of MPEG is 90khz
    ret = avformat_write_header(outputFormatContext, nullptr);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        av_log(outputFormatContext, AV_LOG_ERROR, "avformat_write_header: %s", errorMessage);
        return ret;
    }

    av_dump_format(outputFormatContext, 0, fullFilename, 1);
    return 0;
}

int FFmpegRecorder::decodeInVideo(SwsContext *swsContext, AVPacket *inPacket, AVFrame *frame, AVFrame *yuvFrame) {
    int ret = 0;

    ret = avcodec_send_packet(inVCodecContext, inPacket);
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
    yuvFrame->pts = frame->pts;
    yuvFrame->pkt_pts = frame->pkt_pts;
    yuvFrame->pkt_dts = frame->pkt_dts;
    return ret;
}

int FFmpegRecorder::encodeOutVideo(AVFrame *yuvFrame) {
    int ret = 0;

    ret = avcodec_send_frame(outVCodecContext, yuvFrame);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        av_log(nullptr, AV_LOG_ERROR, "avcodec_send_frame: %s", errorMessage);
        return ret;
    }

    while (ret >= 0) {
        AVPacket outPacket;
        av_init_packet(&outPacket);

        ret = avcodec_receive_packet(outVCodecContext, &outPacket);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            break; // non-enough data, return 0 for continue;
        } else if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(nullptr, AV_LOG_ERROR, "avcodec_receive_packet: %s", errorMessage);
            return ret;
        }

        av_log(nullptr, AV_LOG_FATAL, "outpacket->data: %p, outpacket->size: %d, outpacket->pts: %lld\n",
               outPacket.data, outPacket.size, outPacket.pts);

        av_packet_rescale_ts(&outPacket, inputFormatContext->streams[inVStreamIndex]->time_base, outVStream->time_base);
        outPacket.duration = 3000;
        outPacket.stream_index = inVStreamIndex;

        av_interleaved_write_frame(outputFormatContext, &outPacket);
        av_packet_unref(&outPacket);
    }

    return ret == AVERROR_EOF ? 1 : 0;
}

int FFmpegRecorder::processRecord() {
    int ret = 0;
    int64_t inPacketOffset = 0;

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

    AVPacket inPacket;
    AVFrame *frame = av_frame_alloc();
    AVFrame *yuvFrame = av_frame_alloc();
    av_image_alloc(yuvFrame->data, yuvFrame->linesize, inVCodecContext->width, inVCodecContext->height,AV_PIX_FMT_YUV420P, 32);

    while (abortSignal == -1) {
        ret = av_read_frame(inputFormatContext, &inPacket);
        if (ret == AVERROR(EAGAIN)) {
            continue;
        } else if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(nullptr, AV_LOG_ERROR, "av_read_frame: %s", errorMessage);
            break;
        }

        if (inPacketOffset == 0) {
            inPacketOffset = inPacket.pts;
        }
        inPacket.pts = inPacket.pts - inPacketOffset;

//        av_log(nullptr, AV_LOG_FATAL, "INPUT packet: pts->%lld, size->%d, data->%p\n", inPacket.pts, inPacket.size, inPacket.data);

        if (inPacket.stream_index == inVStreamIndex) {
            ret = decodeInVideo(swsContext, &inPacket, frame, yuvFrame);
            if (ret < 0) return ret;

            ret = encodeOutVideo(yuvFrame);
            if (ret < 0) return ret;

            av_packet_unref(&inPacket);
        } else if (inPacket.stream_index == inAStreamIndex) {
            // todo
        }
    }

    av_write_trailer(outputFormatContext);
    if (!(outputFormatContext->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_close(outputFormatContext->pb);
        if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            av_log(outputFormatContext, AV_LOG_ERROR, "avio_close: %s", errorMessage);
            return ret;
        }
    }

    av_frame_free(&frame);
    av_frame_free(&yuvFrame);
    sws_freeContext(swsContext);
    return ret;
}

void FFmpegRecorder::deallocate() {
    avformat_close_input(&inputFormatContext);
    avformat_free_context(outputFormatContext);
    avcodec_close(inVCodecContext);
    avcodec_close(outVCodecContext);
    options = nullptr;

    isRunning = -1;
}
