//
// Created by justin on 2021/02/03.
//

#include "FFmpegDecoder.h"

const std::string diskPath = "/Users/justin/Downloads/";
AVPixelFormat dstFormat = AV_PIX_FMT_RGB24;
//
//std::string saveImage(AVFrame *pFrame, int width, int height, const std::string &filename) {
//    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
//            std::chrono::system_clock::now().time_since_epoch()
//    );
//    const std::string fullFilename = filename + std::to_string(ms.count()) + ".yuv";
//
////    std::ofstream ofs(fullFilename, std::ios_base::out | std::ios_base::binary);
//    std::ofstream outFile(fullFilename, std::ofstream::binary);
//
//    // write header
//    outFile << "P6\n" << width << " " << height << "\n" << "255\n";
//    // Write pixel data
//    for (int y = 0; y < height; y++) {
//        outFile.write((const char *) pFrame->data[0],  );
//        outFile.write((const char *) pFrame->data[0] + y * pFrame->linesize[0], width);
//    }
//
//    outFile.close();
//    return fullFilename;
//}

void delay(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

FFmpegDecoder::FFmpegDecoder() {}

FFmpegDecoder::~FFmpegDecoder() {}

void FFmpegDecoder::setScreen(QLabel *label) {
    this->screen = label;
}

int FFmpegDecoder::decodeMultimediaFile(const std::string &filename) {
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
    if (videoStreamIndex < 0) return -1;

    audioStreamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, &audioCodec, -1);
    if (audioStreamIndex < 0) return -1;

    return 0;
}

int FFmpegDecoder::openCodec() {
    int ret;

    videoCodecContext = avcodec_alloc_context3(videoCodec);
    ret = avcodec_parameters_to_context(videoCodecContext, formatContext->streams[videoStreamIndex]->codecpar);
    if (ret < 0) return ret;
    ret = avcodec_open2(videoCodecContext, videoCodec, nullptr);
    if (ret != 0) return ret;

    audioCodecContext = avcodec_alloc_context3(audioCodec);
    ret = avcodec_parameters_to_context(audioCodecContext, formatContext->streams[audioStreamIndex]->codecpar);
    if (ret < 0) return ret;
    ret = avcodec_open2(audioCodecContext, audioCodec, nullptr);
    if (ret != 0) return ret;

    return 0;
}

int FFmpegDecoder::beginDecode() {
    swsContext = sws_alloc_context();
    swsContext = sws_getContext(
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
    av_image_alloc(
            retFrame->data, retFrame->linesize,
            videoCodecContext->width, videoCodecContext->height,
            dstFormat, 32
    );
/*    int nBytes = av_image_get_buffer_size(dstFormat, videoCodecContext->width, videoCodecContext->height, 32);
    buffer = (uint8_t *) av_malloc(nBytes * sizeof(uint8_t));

    av_image_fill_arrays(
            retFrame->data,
            retFrame->linesize,
            buffer,
            dstFormat,
            videoCodecContext->width,
            videoCodecContext->height,
            32
    );*/

    return 0;
}

int FFmpegDecoder::decode() {
    int ret, times = 0;
//    std::ofstream outfile("decode_audio.pcm", std::ios::out | std::ios::binary);

    while ((ret = av_read_frame(formatContext, packet)) == 0) {
        if (ret != 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            std::cout << "av_read_frame: " << errorMessage << std::endl;
            return ret;
        }
        if (packet->stream_index == videoStreamIndex) {
            ret = decodeVideo();
            if (ret < 0) return ret;
        }
//        if (packet->stream_index == audioStreamIndex) {
//            ret = decodeAudio(&outfile);
//
//            if (ret < 0) return ret;
//        }

        av_packet_unref(packet);
    }

//    outfile.close();
    return 0;
}

int FFmpegDecoder::decodeAudio(std::ofstream *outfile) {
    int ret, i, channel, dataSize;
    ret = avcodec_send_packet(audioCodecContext, packet);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        std::cout << "avcodec_send_packet: " << errorMessage << std::endl;
        return ret;
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(audioCodecContext, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
             return 0;
        } else if (ret < 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            std::cout << "avcodec_receive_frame: " << errorMessage << std::endl;
            return ret;
        }
        dataSize = av_get_bytes_per_sample(audioCodecContext->sample_fmt);
        for (i = 0; i < frame->nb_samples; ++i) {
            for (channel = 0; channel < audioCodecContext->channels; ++channel) {
                // todo write data to file
                outfile->write((char *) frame->data[channel] + dataSize*i, dataSize);
//                fwrite(frame->data[channel] + dataSize*i, 1, dataSize, outfile);
            }
        }
    }
    return 0;
}

int FFmpegDecoder::decodeVideo() {
    int ret;

    ret = avcodec_send_packet(videoCodecContext, packet);
    if (ret != 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        std::cout << "avcodec_send_packet: " << errorMessage << std::endl;
        return ret;
    }

    ret = avcodec_receive_frame(videoCodecContext, frame);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
        return 0;
    } else if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        std::cout << "avcodec_receive_frame: " << errorMessage << std::endl;
        return ret;
    }

    sws_scale(
            swsContext,
            (uint8_t const *const *) frame->data,
            frame->linesize,
            0,
            videoCodecContext->height,
            retFrame->data,
            retFrame->linesize
    );

    std::cout << " frame->pts: " << frame->pts << std::endl;
//            char buff[100];
//            snprintf(buff, sizeof(buff), "%s%d_%lld_", diskPath.c_str(), times, frame->pts);
//            std::string filename = buff;
//
//            std::string filepath = saveImage(frame, videoCodecContext->width, videoCodecContext->height, filename);

    QImage img((uchar *) retFrame->data[0], videoCodecContext->width, videoCodecContext->height,
               QImage::Format_RGB888);
    screen->setPixmap(QPixmap::fromImage(img));
    delay(40);

    return 0;
}

int FFmpegDecoder::deallocate() {
    avformat_close_input(&formatContext);
    avcodec_free_context(&videoCodecContext);
    av_frame_free(&frame);
    av_frame_free(&retFrame);
    av_packet_free(&packet);
    sws_freeContext(swsContext);
    return 0;
}
