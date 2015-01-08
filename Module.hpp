#pragma once

#include "Region.hpp"
#include "Handle.hpp"
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <string.h>

namespace remote {
    struct ModuleSymbol
    {
        ModuleSymbol() {
            offset = 0;
        }

        off_t offset;
        std::string name;
    };

    class Module
    {
    public:
        typedef enum {
            ARCH_32,
            ARCH_64
        } eArch;

        Module() {
            regions.clear();
        }

        Module(std::vector<remote::Region> regs) {
            regions = regs;
        }

        bool IsValid() {
            return (GetRegionCount() > 0);
        }

        std::string GetPath() {
            return (*regions.begin()).pathname;
        }

        std::string GetFileName() {
            return (*regions.begin()).filename;
        }

        size_t GetStart() {
            return regions.begin()->start;
        }

        size_t GetEnd() {
            return regions.end()->end;
        }

        size_t GetRegionCount() {
            return regions.size();
        }

        Region GetRegionByIndex(size_t index) {
            return regions.at(index);
        }

        void CacheSymbolData();

        size_t GetSymbolAddress(std::string name);

    private:
        void CacheSymbolData32(void* buffer);
        void CacheSymbolData64(void* buffer);
        //

        std::vector<remote::Region> regions;
        std::vector<remote::ModuleSymbol> symbols;
    };

    Module GetModuleHandle(remote::Handle proc, std::string filename);
};