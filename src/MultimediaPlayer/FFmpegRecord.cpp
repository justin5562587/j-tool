//
// Created by justin on 2021/01/19.
//

#include "FFmpegRecord.h"

FFmpegRecord::FFmpegRecord() {
    avdevice_register_all();
}

FFmpegRecord::~FFmpegRecord() {

}

int FFmpegRecord::openCamera() {
    options = nullptr;
    pAVFormatContext = avformat_alloc_context();

    av_dict_set(&options, "framerate", "30", 0);

    // use avfoundation here
    pAVInputFormat = av_find_input_format("avfoundation");
    avformat_open_input(&pAVFormatContext, "0:0", pAVInputFormat, &options);

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
int FFmpegRecord::initializeOutputFile() {
    outAVFormatContext = nullptr;
    value = 0;
    outputFile = "../media/output.mp4";

    avformat_alloc_output_context2(&outAVFormatContext, nullptr, nullptr, outputFile);

    // return output format in the registered format list which best match the parameters
    outAVOutputFormat = av_guess_format(nullptr, outputFile, nullptr);

    video_st = avformat_new_stream(outAVFormatContext, nullptr);

    outAVCodecContext = avcodec_alloc_context3(outAVCodec);

    // set properties of output video file
    avcodec_parameters_to_context(outAVCodecContext, video_st->codecpar); // outAVCodecContext = video_st->codec;
    outAVCodecContext->codec_id = AV_CODEC_ID_MPEG4;
    outAVCodecContext->codec_type = AVMEDIA_TYPE_VIDEO;
    outAVCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    outAVCodecContext->bit_rate = 400000;
    outAVCodecContext->width = 1920;
    outAVCodecContext->height = 1080;
    outAVCodecContext->gop_size = 3;
    outAVCodecContext->max_b_frames = 2;
    outAVCodecContext->time_base.num = 1;
    outAVCodecContext->time_base.den = 30;

    if (codecId == AV_CODEC_ID_H264) {
        av_opt_set(outAVCodecContext->priv_data, "present", "slow", 0);
    }

    outAVCodec = avcodec_find_decoder(AV_CODEC_ID_MPEG4);

    // some containers require global header (like mp4) to be present
    // Mart the decoder so that it behaves accordingly
    if (outAVFormatContext->oformat->flags & AVFMT_GLOBALHEADER) {
        outAVCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    avcodec_open2(outAVCodecContext, outAVCodec, nullptr);

    // create empty video file
    if (!(outAVFormatContext->flags & AVFMT_NOFILE)) {
        if (avio_open2(&outAVFormatContext->pb, outputFile, AVIO_FLAG_WRITE, nullptr, nullptr) < 0) {
            std::cout << "\nerror in create new file";
            return -1;
        }
    }

    if (!outAVFormatContext->nb_streams) {
        std::cout << "\noutput file include no streams";
        return -1;
    }

    value = avformat_write_header(outAVFormatContext, &options);
    if (value < 0) {
        std::cout << "\nerror in write header";
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
    int nb_frames = 100; // numbers of frames to capture

    AVPacket outPacket;
    int j = 0;

    while (av_read_frame(pAVFormatContext, pAVPacket) > 0) {
        if (++ii == nb_frames) break;
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
        av_packet_unref(&outPacket);
    }

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
