#include "FileSystem.hpp"

using namespace std;
using namespace FileSystem;

string FileSystem::ReadFileToEnd(string file) {
    std::ifstream t(file);
    std::string str;

    t.seekg(0, std::ios::end);
    std::streampos pos = t.tellg();
    str.reserve((size_t) pos); // This is probably not a great idea
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    return str;
}

string FileSystem::ReadFile(string file, size_t size) {
    return string();
}

void* FileSystem::ReadFileRawToEnd(string file) {
    off_t fileSize = GetFileSize(file);

    if(fileSize == 0)
        return NULL;

    void* allocatedBuffer = malloc((size_t) fileSize);

    if(allocatedBuffer == NULL)
        return NULL;

    FILE* fp = fopen(file.c_str(), "r");

    if(fp) {
        size_t readSize = fread(allocatedBuffer, 1, (size_t) fileSize, fp);

        fclose(fp);

        if(readSize == fileSize) {
            return allocatedBuffer;
        }
    }

    free(allocatedBuffer);

    return NULL;
}

off_t FileSystem::GetFileSize(std::string filename) {
    struct stat sts;
    int rc = stat(filename.c_str(), &sts);

    if(rc == 0 || rc == -1)
        return 0;

    return sts.st_size;
}

bool FileSystem::WriteFile(string file, string data) {
    return false;
}

bool FileSystem::AppendFile(string file, string data) {
    return false;
}

string FileSystem::ResolveSymLink(string path) {
    char buf[PATH_MAX];

    ssize_t len = ::readlink(path.c_str(), buf, sizeof(buf) - 1);

    if(len != -1) {
        buf[len] = 0;

        return string(buf);
    }

    return string();
}