#include "Elf.hpp"

Elf32_Ehdr* elf::arch32::GetHeader(void* data) {
    Elf32_Ehdr* ret = (Elf32_Ehdr*) data;

    if(memcmp(ret->e_ident, ELFMAG, SELFMAG) != 0)
        return NULL; // Invalid ELF

    if(GetClass(data) != ELFCLASS32)
        return NULL; // x64 only

    return ret;
}

Elf32_Shdr* elf::arch32::GetSectionHeaderEntry(void* data, size_t index) {
    Elf32_Ehdr* header = GetHeader(data);

    if(header == NULL)
        return NULL;

    return (Elf32_Shdr*)((uint8_t *) data + (header->e_shoff + (header->e_shentsize * index)));
}

size_t elf::arch32::GetSectionHeaderEntryCount(void* data) {
    return 0;
}