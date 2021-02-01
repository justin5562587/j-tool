//
// Created by justin on 2021/01/28.
//

#include "FFmpegFilter.h"

const char *filterDescr = "scale=78:24,transpose=cclock";
enum AVPixelFormat pix_fmts[] = {AV_PIX_FMT_GRAY8, AV_PIX_FMT_NONE};

int saveImage(AVFrame *pFrame, int width, int height, const std::string &diskPath) {
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    const std::string fullFilename = diskPath + "_screenshot_" + std::to_string(ms.count()) + ".ppm";
    std::ofstream ofs(fullFilename, std::ios_base::out | std::ios_base::binary);
    // write header
    ofs << "P6\n" << width << " " << height << "\n" << "255\n";
    // Write pixel data
    for (int y = 0; y < height; y++) {
        ofs.write((const char *) pFrame->data[0] + y * pFrame->linesize[0], width * 3);
    }

    ofs.close();
    return 0;
}

FFmpegFilter::FFmpegFilter() {}

FFmpegFilter::~FFmpegFilter() {}

int FFmpegFilter::initializeOpenFile(const std::string &filepath) {
    int ret = 0;
    avformat_open_input(&formatContext, filepath.c_str(), nullptr, nullptr);
    avformat_find_stream_info(formatContext, nullptr);

    for (int i = 0; i < formatContext->nb_streams; ++i) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndexStream = i;
            break;
        }
    }

//    videoIndexStream = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &codec, 0);

    codec = avcodec_find_decoder(formatContext->streams[videoIndexStream]->codecpar->codec_id);

    // create decode context
    codecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecContext, formatContext->streams[videoIndexStream]->codecpar);
    avcodec_open2(codecContext, codec, nullptr);

    return ret;
}

int FFmpegFilter::initializeFilter(const char *filtersDescr) {
    int ret = 0;
    char args[512];
    buffersrc = avfilter_get_by_name("buffer");
    buffersink = avfilter_get_by_name("buffersink");
    outputs = avfilter_inout_alloc();
    inputs = avfilter_inout_alloc();
    AVRational timebase = formatContext->streams[videoIndexStream]->time_base;

    filterGraph = avfilter_graph_alloc();
    if (!outputs || !inputs || !filterGraph) {
        ret = AVERROR(ENOMEM);
        deallocateInOut();
        return ret;
    }

    // format args with parameters
    snprintf(args, sizeof(args),
             "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
             codecContext->width, codecContext->height, codecContext->pix_fmt,
             timebase.num, timebase.den,
             codecContext->sample_aspect_ratio.num, codecContext->sample_aspect_ratio.den);

    // buffer video source
    ret = avfilter_graph_create_filter(&buffersrcContext, buffersrc, "in", args, nullptr, filterGraph);
    if (ret < 0) {
        std::cout << "\ncannot create buffer source";
        return ret;
    }

    // buffer video sink
    ret = avfilter_graph_create_filter(&buffersinkContext, buffersink, "out", nullptr, nullptr, filterGraph);
    if (ret < 0) {
        std::cout << "\ncannot create buffer sink";
        return ret;
    }

    ret = av_opt_set_int_list(buffersinkContext, "pix_fmts", pix_fmts, AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        std::cout << "\ncannot set output pixel format";
        return ret;
    }

    // set outputs ad buffersrc
    outputs->name = av_strdup("in");
    outputs->filter_ctx = buffersrcContext;
    outputs->pad_idx = 0;
    outputs->next = nullptr;

    // set inputs as buffersink
    inputs->name = av_strdup("out");
    inputs->filter_ctx = buffersinkContext;
    inputs->pad_idx = 0;
    inputs->next = nullptr;

    ret = avfilter_graph_parse_ptr(filterGraph, filtersDescr, &inputs, &outputs, nullptr);
    if (ret < 0) {
        deallocateInOut();
        return ret;
    }

    ret = avfilter_graph_config(filterGraph, nullptr);
    if (ret < 0) {
        deallocateInOut();
        return ret;
    }

    return ret;
}

void FFmpegFilter::deallocate() {
    avfilter_graph_free(&filterGraph);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
    av_frame_free(&frame);
    av_frame_free(&filterFrame);
}

void FFmpegFilter::deallocateInOut() {
    avfilter_inout_free(&inputs);
    avfilter_inout_free(&outputs);
}
//
//static void displayFrame(const AVFrame *frame, AVRational timebase) {
//    int x, y;
//    uint8_t *p0, *p;
//    int64_t delay;
//
//    if (frame->pts != AV_NOPTS_VALUE) {
//        if (last_pts != AV_NOPTS_VALUE) {
//            delay = av_rescale_q(frame->pts - last_pts,
//                                 timebase, AV_TIME_BASE_Q);
//            if (delay > 0 && delay < 1000000)
//                usleep(delay)
//        }
//        last_pts = frame->pts;
//    }
//
//    p0 = frame->data[0];
//    puts("\033c");
//    for (y = 0; y < frame->height; y++) {
//        p = p0;
//        for (x = 0; x < frame->width; x++)
//            putchar(" .-+#"[*(p++) / 52]);
//        putchar('\n');
//        p0 += frame->linesize[0];
//    }
//    fflush(stdout);
//}

int FFmpegFilter::decodeFilterFrames(const std::string &filepath, int nFrames, const std::string &diskPath) {
    int ret = 0;
    AVPacket packet;
    frame = av_frame_alloc();
    filterFrame = av_frame_alloc();

    ret = initializeOpenFile(filepath);
    if (ret < 0) {
        std::cout << "\ninitializeOpenFile failed";
        return ret;
    }
    ret = initializeFilter(filterDescr);
    if (ret < 0) {
        std::cout << "\ninitializeFilter failed";
        return ret;
    }

    while (true) {
        ret = av_read_frame(formatContext, &packet);
        if (ret != 0) {
            av_strerror(ret, errorMessage, sizeof(errorMessage));
            std::cout << "\navcodec_send_packet failed: " << errorMessage;
            break;
        }

        if (packet.stream_index == videoIndexStream) {
            ret = avcodec_send_packet(codecContext, &packet);
            if (ret < 0) {
                av_strerror(ret, errorMessage, sizeof(errorMessage));
                std::cout << "\navcodec_send_packet failed: " << errorMessage;
                break;
            }

            while (ret > 0) {
                ret = avcodec_receive_frame(codecContext, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                } else if (ret < 0) {
                    deallocate();
                    return ret;
                }

                frame->pts = frame->best_effort_timestamp;
                // put the decoded frame int filter_graph
                av_buffersrc_add_frame_flags(buffersrcContext, frame, AV_BUFFERSRC_FLAG_KEEP_REF);

                // pull the filtered frame from filter_graph
                while (true) {
                    ret = av_buffersink_get_frame(buffersinkContext, filterFrame);
                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                        break;
                    }
                    if (ret < 0) {
                        deallocate();
                        return ret;
                    }
                    // todo display or export filtered frame
                    saveImage(filterFrame, codecContext->width, codecContext->height, diskPath);
//                    displayFrame(filterFrame, buffersinkContext->inputs[0]->time_base);
                    av_frame_unref(filterFrame);
                }
                av_frame_unref(frame);
            }
        }
        av_packet_unref(&packet);
    }

    return ret;
}
