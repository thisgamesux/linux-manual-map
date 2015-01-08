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

/*switch( header->e_ident[ 4 ] )
    {
        case 1:     return ELF64_ObjectFileClass32;
        case 2:     return ELF64_ObjectFileClass64;
        default:    break;
    }*/

/*static const char *
get_elf_class (unsigned int elf_class)
{
static char buff[32];

switch (elf_class)
{
case ELFCLASSNONE: return _("none");
case ELFCLASS32:   return "ELF32";
case ELFCLASS64:   return "ELF64";
default:
snprintf (buff, sizeof (buff), _("<unknown: %x>"), elf_class);
return buff;
}
}*/

void elf::Close(void* data) {
    if(data) {
        free(data);
    }
}