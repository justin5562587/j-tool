//
// Created by justin on 2021/4/23.
//

#ifndef J_TOOL_CACHECONTROLLER_H
#define J_TOOL_CACHECONTROLLER_H

#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

class CacheController {

public:

    explicit CacheController();

//    CacheController(const CacheController& cacheController) = delete;

//    CacheController& operator=(const CacheController& cacheController) noexcept;

    ~CacheController();

    void loadCache();

private:

//    void writeCache(std::istream input);
//

//
//    void cleanCache();

//    char *cacheBuffer;
};


#endif //J_TOOL_CACHECONTROLLER_H
