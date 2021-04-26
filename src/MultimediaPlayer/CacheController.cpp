//
// Created by justin on 2021/4/23.
//

#include "CacheController.h"

CacheController::CacheController(const std::string &customFilePath) {
    filepath = customFilePath;
}

CacheController::~CacheController() {
    if (cacheFile.is_open()) {
        cacheFile.close();
    }
}

CacheController& CacheController::operator=(CacheController &&cacheController) noexcept {
    this->filepath = cacheController.filepath;
    this->cacheFile.open(this->filepath);
    return *this;
}

void CacheController::loadCache() {
    cacheFile.open(filepath,std::ios::binary | std::ios::in | std::ios::out);
    if (!cacheFile) {
        std::cout << "cannot open cache file\n";
    }
}

CacheController::ccErrno CacheController::readCache(char *gBuffer) {
    cacheFile.seekg(0, cacheFile.end);
    const int length = cacheFile.tellg();
    if (strlen(gBuffer) < length) {
        return INSUFFICIENT_CHAR_LENGTH;
    }

    cacheFile.read(gBuffer, cacheFile.tellg());
    cacheFile.seekg(0, cacheFile.beg);
    return 0;
}

void CacheController::writeCache(const std::string &input) {
    unsigned long length = input.length() + 1;
    char tmpInputChar[length];
    strcpy(tmpInputChar, &input[0]);
    strcat(tmpInputChar, "\n");
    cacheFile.write(tmpInputChar, length);
    cacheFile.flush();
}

void CacheController::cleanCache(operationFlag flag) {
    if (cacheFile.is_open()) {
        cacheFile.close();
    }
    cacheFile.open(filepath, std::ios::trunc);
    if (flag & CLOSE) {
        cacheFile.close();
    }
}

void CacheController::backupCache() {
    cacheFile.seekg(0, cacheFile.end);
    int length = cacheFile.tellg();
    cacheFile.seekg(0, cacheFile.beg);

    char tmpBuffer[length];
    cacheFile.read(tmpBuffer, length);
    cacheFile.close();

    typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> milliTime;
    milliTime nowDate = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());

    std::ofstream backupFile("backup.txt");
    backupFile.write(tmpBuffer, length);
    nowDate.time_since_epoch().count();
    backupFile << " " << nowDate.time_since_epoch().count() << "\n";
    backupFile.close();
}
