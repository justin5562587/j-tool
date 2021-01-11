//
// Created by justin on 2021/01/08.
//
#pragma once

#include <string>
#include <map>

int getInfoAboutFile(std::map<std::string, std::string> *fileInfo, const std::string &filename);

int getPixmapWithTimestamp(const std::string &filename, int64_t timestamp);
