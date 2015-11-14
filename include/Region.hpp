#pragma once

#include <elf.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <regex>
#include <iostream>

namespace remote
{
    struct Region
    {
        Region() {
            start = end = 0;
            read = write = exec = shared = false;
            offset = deviceMajor = deviceMinor = inodeFileNumber = 0;
            pathname.clear();
            filename.clear();
        }

        size_t start;
        size_t end;

        bool read, write, exec, shared;

        size_t offset;
        size_t deviceMajor, deviceMinor;
        size_t inodeFileNumber;

        std::string pathname;
        std::string filename;
    };

    std::vector<Region> GetRegions(pid_t pid);
};