#pragma once

#include <string>
#include <linux/limits.h>
#include <unistd.h>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>

namespace FileSystem
{
    std::string ReadFileToEnd(std::string file);
    std::string ReadFile(std::string file, size_t size);

    void* ReadFileRawToEnd(std::string file);

    off_t GetFileSize(std::string filename);

    bool WriteFile(std::string file, std::string data);
    bool AppendFile(std::string file, std::string data);

    std::string ResolveSymLink(std::string path);
};