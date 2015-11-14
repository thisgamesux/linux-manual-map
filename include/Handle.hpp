#pragma once

#include <sys/types.h>
#include <sys/uio.h>
#include <string>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>
#include "Region.hpp"

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE (pid_t)-1
#endif

namespace remote
{
    struct Handle
    {
        Handle() : pid(-1) {}
        Handle(pid_t target) : pid(target) {}

        bool IsValid();
        bool IsRunning();

        std::string GetPidString();
        std::string GetProcDirectory();
        std::string GetPath();
        std::string GetWorkingDirectory();

        ssize_t WriteMemory(void* address, void* buffer, size_t size);
        ssize_t ReadMemory(void* address, void* buffer, size_t size);

        size_t GetModuleAddress(std::string name);
        Region GetRegionByAddress(size_t address);
        std::vector<Region> GetRegions();

        Handle operator=(const pid_t setpid) {
            pid = setpid;

            return *this;
        }

        Handle operator=(const Handle setpid) {
            this->pid = setpid.pid;

            return *this;
        }

        bool operator==(const pid_t &comp) {
            return (comp == this->pid);
        }

        bool operator==(const Handle &comp) {
            return (*this == comp.pid);
        }

        bool operator!=(const pid_t &comp) {
            return !(*this == comp);
        }

        bool operator!=(const Handle &comp) {
            return !(*this == comp);
        }

        pid_t pid;
    };

    Handle OpenProcess(pid_t pid);
    Handle OpenProcess(std::string name);

    bool ReadProcessMemory(Handle hProcess, void* lpBaseAddress, void* lpBuffer, size_t nSize, size_t* lpNumberOfBytesRead);
    bool WriteProcessMemory(Handle hProcess, void* lpBaseAddress, void* lpBuffer, size_t nSize, size_t* lpNumberOfBytesWritten);
};