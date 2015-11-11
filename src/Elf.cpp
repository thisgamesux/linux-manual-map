#include <elf.h>
#include "Elf.hpp"

void* elf::Open(const char* filename) {
    if(filename == NULL)
        return NULL;

    FILE* fp = fopen(filename, "r");

    if(!fp) {
        return NULL;
    }

    fseeko(fp, 0, SEEK_END);
    off_t elf_len = ftello(fp);
    rewind(fp);

    if(elf_len == 0) {
        fclose(fp);
        return NULL;
    }

    void* elf_data = malloc((size_t) elf_len);

    if(fread(elf_data, 1, (size_t) elf_len, fp) != elf_len) {
        fclose(fp);
        return NULL;
    }

    fclose(fp);

    return elf_data;
}

uint8_t elf::GetClass(void *data) {
    Elf64_Ehdr* head = (Elf64_Ehdr*) data;

    return (head->e_ident[EI_CLASS]);
}

void elf::Close(void* data) {
    if(data) {
        free(data);
    }
}