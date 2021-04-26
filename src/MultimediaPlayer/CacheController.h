//
// Created by justin on 2021/4/23.
//

#ifndef J_TOOL_CACHECONTROLLER_H
#define J_TOOL_CACHECONTROLLER_H

#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <chrono>

class CacheController {

public:

    typedef unsigned int operationFlag;
    static const operationFlag FLUSH = 0x01;
    static const operationFlag CLOSE = 0x02;
    static const operationFlag OPEN = 0x04;
    static const operationFlag OPEN_FLUSH = FLUSH | OPEN;
    static const operationFlag CLOSE_FLUSH = FLUSH | CLOSE;

    typedef unsigned int ccErrno;
    static const ccErrno INSUFFICIENT_CHAR_LENGTH = 0x01;

    explicit CacheController(const std::string &customFilePath);

    CacheController& operator=(CacheController&& cacheController) noexcept;

    ~CacheController();

    void loadCache();

    ccErrno readCache(char *gBuffer);

    void writeCache(const std::string &input);

    void cleanCache(operationFlag flag = CLOSE);

    void backupCache();

private:

    std::string filepath = "/Users/justin/cpp/j-tool/src/MultimediaPlayer/j_multimedia_player_cache.txt";

    std::fstream cacheFile;
};


#endif //J_TOOL_CACHECONTROLLER_H
