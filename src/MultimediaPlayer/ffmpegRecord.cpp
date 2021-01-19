//
// Created by justin on 2021/01/19.
//

#include "ffmpegRecord.h"


int recordVideoWithFFmpeg(AdvancedControl *advancedControl, const std::string &diskPath) {
    avdevice_register_all();

    int ret;
    char errorMessage[1024];
    char *deviceName = "0";
    AVFormatContext *pFormatCtx = nullptr;
    AVDictionary *options = nullptr;

    av_dict_set(&options, "video_size", "640*480", 0);
    av_dict_set(&options, "framerate", "30", 0);

    // get format
    AVInputFormat *pInputFormat = av_find_input_format("avfoundation");
    if (pInputFormat == nullptr) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        std::cout << "avformat_open_input: " << errorMessage << std::endl;
        return -1;
    }

    ret = avformat_open_input(&pFormatCtx, deviceName, pInputFormat, &options);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        std::cout << "avformat_open_input: " << errorMessage << std::endl;
        return -1;
    }

    int count = 0;
    AVPacket packet;
    av_init_packet(&packet);

    while ((ret = av_read_frame(pFormatCtx, &packet)) == 0 && count < 500) {
        std::cout << "packet.size: " << packet.size << std::endl;
        writeVideoDataToDisk(diskPath, &packet);
        av_packet_unref(&packet);
        ++count;
    }
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        std::cout << "av_read_frame: " << errorMessage << std::endl;
        return -1;
    }

    avformat_close_input(&pFormatCtx);
    av_log(nullptr, AV_LOG_DEBUG, "Record Video Finished.\n");

    // todo callback - need optimize
    advancedControl->setBtnStatus(VIDEO_RECORD, true);

    return 0;
}

// todo
void writeVideoDataToDisk(const std::string &diskPath, AVPacket *packet) {
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    const std::string fullFilename = diskPath + std::to_string(ms.count()) + ".yuv";
    std::ofstream ofs(fullFilename, std::ios_base::out | std::ios_base::binary);

    // Write PCM data
    ofs.write((const char*) packet->data, packet->size);

    ofs.close();
}

int recordAudioWithFFmpeg(AdvancedControl *advancedControl, const std::string &diskPath) {
    avdevice_register_all();

    int ret;
    char errorMessage[1024];
    AVFormatContext *pFormatCtx = nullptr;
    char *deviceName = ":0";

    // get format
    AVInputFormat *pInputFormat = av_find_input_format("avfoundation");
    if (pInputFormat == nullptr) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        std::cout << "avformat_open_input: " << errorMessage << std::endl;
        return -1;
    }

    ret = avformat_open_input(&pFormatCtx, deviceName, pInputFormat, nullptr);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        std::cout << "avformat_open_input: " << errorMessage << std::endl;
        return -1;
    }

    int count = 0;
    AVPacket packet;
    av_init_packet(&packet);

    while ((ret = av_read_frame(pFormatCtx, &packet)) == 0 && count < 500) {
        std::cout << "packet.size: " << packet.size << std::endl;
        writeAudioDataToDisk(diskPath, &packet);
        av_packet_unref(&packet);
        ++count;
    }
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        std::cout << "av_read_frame: " << errorMessage << std::endl;
        return -1;
    }

    avformat_close_input(&pFormatCtx);
    av_log(nullptr, AV_LOG_DEBUG, "Record Audio Finished.\n");

    // todo callback - need optimize
    advancedControl->setBtnStatus(AUDIO_RECORD, true);

    return 0;
}

void writeAudioDataToDisk(const std::string &diskPath, AVPacket *packet) {
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    const std::string fullFilename = diskPath + std::to_string(ms.count()) + ".pcm";
    std::ofstream ofs(fullFilename, std::ios_base::out | std::ios_base::binary);

    // Write PCM data
    ofs.write((const char*) packet->data, packet->size);

    ofs.close();
}
