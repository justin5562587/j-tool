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

    explicit CacheController(const std::string &customFilePath);

    CacheController& operator=(CacheController&& cacheController) noexcept;

    ~CacheController();

    void loadCache();

    void readCache();

    void writeCache(const std::string &input);

    void cleanCache(operationFlag flag = CLOSE);

    void backupCache();

private:

    std::string filepath = "/Users/justin/cpp/j-tool/src/MultimediaPlayer/j_multimedia_player_cache.txt";

    std::fstream cacheFile;
};


#endif //J_TOOL_CACHECONTROLLER_H
