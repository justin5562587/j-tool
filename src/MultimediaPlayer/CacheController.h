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

    explicit CacheController(const std::string &customFilePath);

    CacheController& operator=(CacheController&& cacheController) noexcept;

    ~CacheController();

    void loadCache();

    void writeCache(const std::string &input);

    void cleanCache();

    void backupCache();

private:

    std::string filepath = "/Users/justin/cpp/j-tool/src/MultimediaPlayer/j_multimedia_player_cache.txt";

    std::fstream cacheFile;
};


#endif //J_TOOL_CACHECONTROLLER_H
