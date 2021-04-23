//
// Created by justin on 2021/4/23.
//

#include "CacheController.h"

CacheController::CacheController() {

}

//CacheController& CacheController::operator=(const CacheController& cacheController) noexcept {
//    memcpy(this->cacheBuffer, cacheController.cacheBuffer, sizeof(cacheController.cacheBuffer));
//    return *this;
//}

CacheController::~CacheController() {
//    delete[] cacheBuffer;
}

//void CacheController::writeCache(char *input) {
//    cacheBuffer
//}

void CacheController::loadCache() {
    std::fstream cacheFile("/Users/justin/cpp/j-tool/src/MultimediaPlayer/j_multimedia_player_cache.txt", std::ios::binary | std::ios::in | std::ios::out);
    if (cacheFile) {
        // get length
        cacheFile.seekg(0, cacheFile.end);
        int length = cacheFile.tellg();
        cacheFile.seekg(0, cacheFile.beg);

        // allocate mem
        char *cacheBuffer = new char[length];

        // read data && close file
        cacheFile.read(cacheBuffer, length);
        cacheFile.close();

        std::fstream resultFile("j_multimedia_player_cache_result.txt");
        resultFile.write(cacheBuffer, length);
        resultFile.close();

        delete[] cacheBuffer;
    }
}