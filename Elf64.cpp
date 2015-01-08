#include <elf.h>
#include "Elf.hpp"

Elf64_Ehdr* elf::arch64::GetHeader(void* data) {
    Elf64_Ehdr* ret = (Elf64_Ehdr*) data;

    if(memcmp(ret->e_ident, ELFMAG, SELFMAG) != 0)
        return NULL; // Invalid ELF

    if(GetClass(data) != ELFCLASS64)
        return NULL; // x64 only

    return ret;
}

Elf64_Shdr* elf::arch64::GetSectionHeaderEntry(void* data, size_t index) {
    Elf64_Ehdr* header = GetHeader(data);

    if(header == NULL)
        return NULL;

    //return (Elf64_Shdr*)((uint8_t *) data + (header->e_shoff + (header->e_shentsize * index)));
    return &((Elf64_Shdr*)((char*) data + header->e_shoff))[index];
}

Elf64_Phdr* elf::arch64::GetProgramHeaderEntry(void *data, size_t index) {
    Elf64_Ehdr* header = GetHeader(data);

    if(header == NULL)
        return NULL;

    return &((Elf64_Phdr*)((char*) data + header->e_phoff))[index];
}

const char* elf::arch64::GetNameOfSectionAtIndex(void* data, Elf64_Shdr* section) {
    uint8_t* sectionData = GetSectionData(data, section);

    if(sectionData == NULL)
        return NULL;

    if(section->sh_name == 0)
        return "<unnamed>";

    return (const char*)((uint8_t*) data + section->sh_name);
}

uint8_t* elf::arch64::GetSectionData(void* data, Elf64_Shdr* section) {
    if(data == NULL || section == NULL)
        return NULL;

    return ((uint8_t*) data + section->sh_offset);
}

size_t elf::arch64::GetSectionHeaderEntryCount(void* data) {
    Elf64_Ehdr* header = GetHeader(data);

    if(header == NULL)
        return 0;

    return header->e_shnum;
}

size_t elf::arch64::GetProgramHeaderEntryCount(void* data) {
    Elf64_Ehdr* header = GetHeader(data);

    if(header == NULL)
        return 0;

    return header->e_phnum;
}