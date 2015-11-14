#pragma once

#include <sys/types.h>
#include <sys/uio.h>
#include "Handle.hpp"

namespace remote
{
    class Loader
    {
    public:
        // Load an ELF library which is on the filesystem
        size_t loadShared(Handle proc, std::string path);

        // Load an ELF library which is only a collection of bytes rather than a file path
        // (Useful for ELF binaries which have already been loaded and read, or over a network stream for example)
        size_t loadSharedFromMemory(Handle proc, size_t local, size_t size);

    private:
        //
    };
};