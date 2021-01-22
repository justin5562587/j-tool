//
// Created by justin on 2021/01/19.
//

#include "FFmpegRecord.h"

FFmpegRecord::FFmpegRecord() {
    avdevice_register_all();
}

FFmpegRecord::~FFmpegRecord() {

}

int FFmpegRecord::initializeRecordDevice() {
    options = nullptr;
    pAVFormatContext = avformat_alloc_context();

    av_dict_set(&options, "framerate", "30", 0);
    av_dict_set(&options, "pixel_format", "uyvy422", 0);
    av_dict_set(&options, "video_size", "1280x720", 0);

    // use avfoundation here
    pAVInputFormat = av_find_input_format("avfoundation");
    avformat_open_input(&pAVFormatContext, "0", pAVInputFormat, &options);

    videoStreamIndex = -1;
    for (int i = 0; i < pAVFormatContext->nb_streams; ++i) {
        if (pAVFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    pAVCodec = avcodec_find_decoder(pAVFormatContext->streams[videoStreamIndex]->codecpar->codec_id);
    pAVCodecContext = avcodec_alloc_context3(pAVCodec);
    avcodec_parameters_to_context(pAVCodecContext, pAVFormatContext->streams[videoStreamIndex]->codecpar);
    avcodec_open2(pAVCodecContext, pAVCodec, nullptr);

    return 0;
}

// initialize output video file
// use AV_CODEC_ID_H264 for test
int FFmpegRecord::initializeOutputFile(AVCodecID avCodecId, const std::string &outputFilePath) {
    value = 0;
    outputFile = outputFilePath.c_str();

    outAVFormatContext = avformat_alloc_context();
    avformat_alloc_output_context2(&outAVFormatContext, nullptr, nullptr, outputFile);

    // return output format in the registered format list which best match the parameters
    outAVOutputFormat = av_guess_format(nullptr, outputFile, nullptr);

    outAVCodec = avcodec_find_decoder(avCodecId);

    video_st = avformat_new_stream(outAVFormatContext, outAVCodec);

    // allocate outAVCodecContext and set properties of output video file
    outAVCodecContext = avcodec_alloc_context3(outAVCodec);
    avcodec_parameters_to_context(outAVCodecContext, video_st->codecpar);
    outAVCodecContext->codec_id = AV_CODEC_ID_MPEG4;
    outAVCodecContext->codec_type = AVMEDIA_TYPE_VIDEO;
    outAVCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    outAVCodecContext->bit_rate = 400000;
    outAVCodecContext->width = 1920;
    outAVCodecContext->height = 1080;
    outAVCodecContext->gop_size = 3;
    outAVCodecContext->max_b_frames = 2;
    outAVCodecContext->framerate = (AVRational) {30, 1};
    outAVCodecContext->time_base = (AVRational) {1, 30};

    if (avCodecId == AV_CODEC_ID_H264) {
        av_opt_set(outAVCodecContext->priv_data, "present", "slow", 0);
    }

    // some containers require global header (like mp4) to be present
    // Mart the decoder so that it behaves accordingly
    if (outAVFormatContext->oformat->flags & AVFMT_GLOBALHEADER) {
        outAVCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    avcodec_open2(outAVCodecContext, outAVCodec, nullptr);

    // create empty video file
    if (!(outAVFormatContext->flags & AVFMT_NOFILE)) {
        value = avio_open2(&outAVFormatContext->pb, outputFile, AVIO_FLAG_WRITE, nullptr, nullptr);
        if (value < 0) {
            av_strerror(value, errorMessage, 1024);
            std::cout << "\navio_open2: " << errorMessage;
            return -1;
        }
    }

    if (!outAVFormatContext->nb_streams) {
        std::cout << "\noutput file include no streams";
        return -1;
    }

    value = avformat_write_header(outAVFormatContext, &options);
    if (value < 0) {
        av_strerror(value, errorMessage, 1024);
        std::cout << "\navformat_write_header: " << errorMessage;
        return -1;
    }

    // dump information about output file
    std::cout << "\n\noutput file information:\n\n";
    av_dump_format(outAVFormatContext, 0, outputFile, 1);

    return 0;
}

int FFmpegRecord::captureVideoFrames() {
    value = 0;

    pAVPacket = (AVPacket *) av_malloc(sizeof(AVPacket));
    av_init_packet(pAVPacket);

    pAVFrame = av_frame_alloc();
    outFrame = av_frame_alloc();

    int nbytes = av_image_get_buffer_size(outAVCodecContext->pix_fmt, outAVCodecContext->width, outAVCodecContext->height, 32);
    uint8_t *video_out_buffer = (uint8_t *) av_malloc(nbytes);

    av_image_fill_arrays(
            outFrame->data,
            outFrame->linesize,
            video_out_buffer,
            AV_PIX_FMT_YUV420P,
            outAVCodecContext->width,
            outAVCodecContext->height,
            1
    );

    SwsContext *swsCtx = sws_getContext(
            pAVCodecContext->width,
            pAVCodecContext->height,
            pAVCodecContext->pix_fmt,
            outAVCodecContext->width,
            outAVCodecContext->height,
            outAVCodecContext->pix_fmt,
            SWS_BICUBIC,
            nullptr, nullptr, nullptr
    );

    int ii = 0;
    int readFrameTimes = 100; // call times of av_read_frame

    AVPacket outPacket;
    while (av_read_frame(pAVFormatContext, pAVPacket) > 0) {
        if (++ii == readFrameTimes) break;
        if (pAVPacket->stream_index == videoStreamIndex) {
            avcodec_send_packet(pAVCodecContext, pAVPacket);
            avcodec_receive_frame(pAVCodecContext, pAVFrame);

            sws_scale(
                    swsCtx,
                    pAVFrame->data,
                    pAVFrame->linesize,
                    0,
                    pAVCodecContext->height,
                    outFrame->data,
                    outFrame->linesize
            );
            av_init_packet(&outPacket);
            outPacket.data = nullptr;
            outPacket.size = 0;

            avcodec_send_frame(outAVCodecContext, outFrame);
            avcodec_receive_packet(outAVCodecContext, &outPacket);

            av_packet_unref(&outPacket);
        }
    }

    std::cout << "\niterate times: " << ii;

    av_write_trailer(outAVFormatContext);

    av_free(video_out_buffer);

    return 0;
}

int FFmpegRecord::cleanAll() {
    avformat_close_input(&pAVFormatContext);
    avformat_close_input(&outAVFormatContext);

    avformat_free_context(pAVFormatContext);
    avformat_free_context(outAVFormatContext);
    return 0;
}

int FFmpegRecord::recordVideo(AVCodecID avCodecId, const std::string &outputFilePath) {
    if (initializeRecordDevice() < 0) {
        std::cout << "\ninitializeRecordDevice failed";
        return -1;
    }
    if (initializeOutputFile(avCodecId, outputFilePath) < 0) {
        std::cout << "\ninitializeOutputFile failed";
        return -1;
    }
    if (captureVideoFrames() < 0) {
        std::cout << "\ncaptureVideoFrames";
        return -1;
    }
    cleanAll();
    return 0;
}
