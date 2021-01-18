//
// Created by justin on 2021/01/18.
//

#include "ffmpegResample.h"

int recordAudioWithFFmpeg() {
    avdevice_register_all();

    int ret;
    char errorMessage[1024];
    AVFormatContext *pFormatCtx = nullptr;
    char *deviceName = ":0";

    char *diskPath = "/User/justin/Downloads/";

    // get format
    AVInputFormat *pInputFormat = av_find_input_format("avfoundation");
    ret = avformat_open_input(&pFormatCtx, deviceName, pInputFormat, nullptr);
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        std::cout << "avformat_open_input: " << errorMessage << std::endl;
    }

    int count = 0;
    AVPacket packet;
    av_init_packet(&packet);
    while ((ret = av_read_frame(pFormatCtx, &packet)) == 0 && count < 500) {
        std::cout << "packet.size: " << packet.size << std::endl;
        writeDataToDisk(diskPath, &packet);
        av_packet_unref(&packet);
        ++count;
    }
    if (ret < 0) {
        av_strerror(ret, errorMessage, sizeof(errorMessage));
        std::cout << "av_read_frame: " << errorMessage << std::endl;
    }

    avformat_close_input(&pFormatCtx);
    av_log(nullptr, AV_LOG_DEBUG, "Record Audio Finished.\n");

    return 0;
}

void writeDataToDisk(const std::string &diskPath, AVPacket *packet) {
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    const std::string fullFilename = diskPath + std::to_string(ms.count()) + ".pcm";
    std::ofstream ofs(fullFilename, std::ios_base::out | std::ios_base::binary);

    // Write PCM data
    ofs.write((const char*) packet->data, packet->size);

    ofs.close();
}
