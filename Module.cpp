#include <elf.h>
#include "Module.hpp"
#include "Elf.hpp"

using namespace std;
using namespace remote;

void remote::Module::CacheSymbolData32(void* buffer) {
    std::cout << "CacheSymbolData32" << std::endl;
}

void remote::Module::CacheSymbolData64(void* buffer) {
    std::cout << "CacheSymbolData64" << std::endl;

    size_t programCount = elf::arch64::GetProgramHeaderEntryCount(buffer);

    for(size_t i = 0; i < programCount; ++i) {
        Elf64_Phdr *prog = elf::arch64::GetProgramHeaderEntry(buffer, i);

        if (!prog)
            continue;

        std::cout << "Prog: " << std::hex << prog->p_offset << ":" << prog->p_paddr << ":" << prog->p_vaddr << ":" << prog->p_type << ":" << prog->p_flags << std::endl;

        switch (prog->p_type) {
            case PT_NULL:
                std::cout << "[PT_NULL]" << std::endl;
                break;
            case PT_LOAD:
                std::cout << "[PT_LOAD]" << std::endl;
                break;
            case PT_DYNAMIC:
                std::cout << "[PT_DYNAMIC]" << std::endl;
                break;
            case PT_INTERP:
                std::cout << "[PT_INTERP]" << std::endl;
                break;
            case PT_NOTE:
                std::cout << "[PT_NOTE]" << std::endl;
                break;
            case PT_SHLIB:
                std::cout << "[PT_SHLIB]" << std::endl;
                break;
            case PT_PHDR:
                std::cout << "[PT_PHDR]" << std::endl;
                break;
            case PT_TLS:
                std::cout << "[PT_TLS]" << std::endl;
                break;
            case PT_LOOS:
                std::cout << "[PT_LOOS]" << std::endl;
                break;
            case PT_HIOS:
                std::cout << "[PT_HIOS]" << std::endl;
                break;
            case PT_LOPROC:
                std::cout << "[PT_LOPROC]" << std::endl;
                break;
            case PT_HIPROC:
                std::cout << "[PT_HIPROC]" << std::endl;
                break;
            case PT_GNU_EH_FRAME:
                std::cout << "[PT_GNU_EH_FRAME]" << std::endl;
                break;
            case PT_GNU_STACK:
                std::cout << "[PT_GNU_STACK]" << std::endl;
                break;
            case PT_GNU_RELRO:
                std::cout << "[PT_GNU_RELRO]" << std::endl;
                break;
            default:
                std::cout << "[Default]" << std::endl;
                break;
        }

        std::string attrString;

        attrString.append((prog->p_flags & PF_R) ? "r" : "-");
        attrString.append((prog->p_flags & PF_W) ? "w" : "-");
        attrString.append((prog->p_flags & PF_X) ? "x" : "-");

        std::cout << attrString << std::endl;
    }

    size_t sectionCount = elf::arch64::GetSectionHeaderEntryCount(buffer);

    std::cout << "Section Count: " << sectionCount << std::endl;

    for(size_t i = 0; i < sectionCount; i++) {
        Elf64_Shdr* sec = elf::arch64::GetSectionHeaderEntry(buffer, i);

        if(!sec)
            continue;

        switch(sec->sh_type) {
            case SHT_SYMTAB:
                std::cout << "Section [" << i << "]: " << sec->sh_offset << " - " << sec->sh_addr << " - SYMTAB (" << sec->sh_type << ")" << std::endl;
                break;
            case SHT_STRTAB:
                std::cout << "Section [" << i << "]: " << sec->sh_offset << " - " << sec->sh_addr << " - STRTAB (" << sec->sh_type << ")" << std::endl;
                break;
            case SHT_REL:
                std::cout << "Section [" << i << "]: " << sec->sh_offset << " - " << sec->sh_addr << " - REL (" << sec->sh_type << ")" << std::endl;
                break;
        }

        /*
        const char* name = elf::arch64::GetNameOfSectionAtIndex(buffer, sec);

        if(!name)
            continue;

        std::cout << "Section: " << name << std::endl;*/
    }
}

/*	size = ehdr.e_shentsize * ehdr.e_shnum;
	shdr = (Elf32_Shdr *) xmalloc(size);
	rv = pread(fd, shdr, size, ehdr.e_shoff);
	if (0 > rv) {
		perror("read");
		goto out;
	}
	if (rv != size) {
		fprintf(stderr, "elf error");
		goto out;
	}

size = shdr[ehdr.e_shstrndx].sh_size;
shstrtab = (char *) xmalloc(size);
rv = pread(fd, shstrtab, size, shdr[ehdr.e_shstrndx].sh_offset);
if (0 > rv) {
perror("read");
goto out;
}
if (rv != size) {
fprintf(stderr, "elf error\n");
goto out;
}

symh = dynsymh = NULL;
strh = dynstrh = NULL;
for (i = 0, p = shdr; i < ehdr.e_shnum; i++, p++)
if (SHT_SYMTAB == p->sh_type) {
if (symh) {*/

void remote::Module::CacheSymbolData() {
    if(GetPath().length() == 0) {
        std::cout << "No path!" << std::endl;
        return;
    }

    void* elf_data = elf::Open(GetPath().c_str());

    if(elf_data == NULL)
        return;

    // We now have elf data!
    // We can check the 'basic' variables like magic and machine without proper 32/64 alignment
    // We re-cast later when we have determined the proper arch
    Elf64_Ehdr ehdr = *(Elf64_Ehdr*) elf_data;

    if(memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
        std::cout << "ELF header magic failed" << std::endl;
        free(elf_data);
        return;
    }
    std::cout << "ELF OK!" << std::endl;

    if(ehdr.e_machine == EM_IA_64 || ehdr.e_machine == EM_X86_64) {
        CacheSymbolData64(elf_data);
    } else if(ehdr.e_machine == EM_M32) {
        CacheSymbolData32(elf_data);
    } else {
        std::cout << "Arch problems??? Fgt." << std::endl;
    }

    elf::Close(elf_data);
}

size_t remote::Module::GetSymbolAddress(std::string name) {
    return 0;
}

Module remote::GetModuleHandle(Handle proc, string filename) {
    vector<Region> processRegions = proc.GetRegions();

    if(processRegions.size() == 0) {
        return Module();
    }

    vector<Region> regs;

    for(auto reg : processRegions) {
        if(reg.filename.compare(filename) == 0) {
            regs.push_back(reg);
        }
    }

    Module ret = Module(regs);

    if(ret.IsValid()) {
        std::cout << "Regions: " << std::hex << ret.GetRegionCount() << std::endl;
        std::cout << "Path: " << ret.GetPath() << std::endl;

        ret.CacheSymbolData();
    }

    return ret;
}