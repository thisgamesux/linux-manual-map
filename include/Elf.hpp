#pragma once

#include <elf.h>
#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include <string.h>
#include <elf.h>
#include <iostream>

namespace elf
{
    void* Open(const char* filename);
    uint8_t GetClass(void* data);
    void Close(void* data);

    namespace arch64
    {
        // Access Pointers
        Elf64_Ehdr* GetHeader(void* data);
        Elf64_Shdr* GetSectionHeaderEntry(void* data, size_t index);
        Elf64_Phdr* GetProgramHeaderEntry(void* data, size_t index);
        const char* GetNameOfSectionAtIndex(void* data, Elf64_Shdr* section);
        uint8_t* GetSectionData(void* data, Elf64_Shdr* section);

        // Count
        size_t GetSectionHeaderEntryCount(void* data);
        size_t GetProgramHeaderEntryCount(void* data);
    };

    namespace arch32
    {
        Elf32_Ehdr* GetHeader(void* data);
        Elf32_Shdr* GetSectionHeaderEntry(void* data, size_t index);
        size_t GetSectionHeaderEntryCount(void* data);
    };
};