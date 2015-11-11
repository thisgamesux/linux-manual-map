#include <string.h>
#include "Handle.hpp"
#include "Region.hpp"
#include "FileSystem.hpp"

using namespace remote;
using namespace std;

bool Handle::IsValid() {
    return (pid != -1);
}

bool Handle::IsRunning() {
    if(IsValid()) {
        struct stat sts;
        return !(stat(GetProcDirectory().c_str(), &sts) == -1 && errno == ENOENT);
    }

    return false;
}

string Handle::GetPidString() {
    stringstream ss;
    ss << dec << pid;
    return ss.str();
}

string Handle::GetProcDirectory() {
    return ("/proc/" + GetPidString());
}

string Handle::GetPath() {
    return FileSystem::ResolveSymLink(GetProcDirectory() + "/exe");
}

string Handle::GetWorkingDirectory() {
    return FileSystem::ResolveSymLink(GetProcDirectory() + "/cwd");
}

ssize_t Handle::WriteMemory(void* address, void* buffer, size_t size) {
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;

    return process_vm_writev(pid, local, 1, remote, 1, 0);
}

ssize_t Handle::ReadMemory(void* address, void* buffer, size_t size) {
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;

    return process_vm_readv(pid, local, 1, remote, 1, 0);
}

size_t Handle::GetModuleAddress(string name) {
    if(IsRunning()) {
        vector<Region> regions = GetRegions();

        // These are ordered by memory start in maps
        // So first result is fine
        for(auto reg : regions) {
            if(reg.filename.compare(name) == 0) {
                return reg.start;
            }
        }
    }

    return 0;
}

Region Handle::GetRegionByAddress(size_t address) {
    Region r;

    if(IsValid() && IsRunning()) {
        vector<Region> regions = GetRegions();

        for(auto reg : regions) {
            if(reg.start >= address && address <= reg.end) {
                r = reg;
            }
        }
    }

    return r;
}

std::vector<Region> Handle::GetRegions() {
    return remote::GetRegions(this->pid);
}

Handle remote::OpenProcess(pid_t pid) {
    Handle ret(pid);

    if(ret.IsRunning())
        return ret;

    return INVALID_HANDLE_VALUE;
}

Handle remote::OpenProcess(string name) {
    DIR* dir = opendir("/proc/");

    if(dir) {
        dirent* dire = NULL;

        while((dire = readdir(dir)) != NULL) {
            if(dire->d_type != DT_DIR)
                continue;

            // Check numeric
            if(strspn(dire->d_name, "0123456789") != strlen(dire->d_name))
                continue;

            char maps[PATH_MAX];
            strncpy(maps, "/proc/", PATH_MAX);
            strncat(maps, dire->d_name, PATH_MAX);
            strncat(maps, "/maps", PATH_MAX);

            if (access(maps, F_OK) == -1)
                continue;

            pid_t pid;

            stringstream ss;
            ss << dec << dire->d_name;
            ss >> pid;

            Handle hProcess = OpenProcess(pid);

            if(hProcess != INVALID_HANDLE_VALUE) {
                string path = hProcess.GetPath();

                size_t lastSlash = path.find_last_of('/');

                if(lastSlash == -1)
                    continue;

                if(path.substr(lastSlash, path.size()).compare(name) == 0)
                    return hProcess;
            }
        }
    }

    return INVALID_HANDLE_VALUE;
}

bool remote::ReadProcessMemory(Handle hProcess, void* lpBaseAddress, void* lpBuffer, size_t nSize, size_t* lpNumberOfBytesRead) {
    if(!hProcess.IsRunning())
        return false;

    // Try to prevent them reading or writing to invalid memory
    Region reg = hProcess.GetRegionByAddress((size_t) lpBaseAddress);

    if(reg.start + reg.end == 0)
        return false;

    ssize_t result = hProcess.ReadMemory(lpBaseAddress, lpBuffer, nSize);

    if(result == -1)
        return false;

    if(lpNumberOfBytesRead)
        *lpNumberOfBytesRead = (size_t) result;

    return (((size_t) result) == nSize);
}

bool remote::WriteProcessMemory(Handle hProcess, void* lpBaseAddress, void* lpBuffer, size_t nSize, size_t* lpNumberOfBytesWritten) {
    if(!hProcess.IsRunning())
        return false;

    // Try to prevent them reading or writing to invalid memory
    Region reg = hProcess.GetRegionByAddress((size_t) lpBaseAddress);

    if(reg.start + reg.end == 0)
        return false;

    ssize_t result = hProcess.WriteMemory(lpBaseAddress, lpBuffer, nSize);

    if(result == -1)
        return false;

    if(lpNumberOfBytesWritten)
        *lpNumberOfBytesWritten = (size_t) result;

    return (((size_t) result) == nSize);
}
