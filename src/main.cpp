#include "main.hpp"

int main(int argc, char **argv) {

    if(argc != 2) {
        std::cout << "Error: please provide a process id!\n";
        return -1;
    }

    remote::Handle proc = remote::OpenProcess(atoi(argv[1]));
 
    if(!proc.IsRunning()) {
        std::cout << "Error: process is not running!\n";
        return -1; 
    }

    std::vector<remote::Region> rs = remote::GetRegions(proc.pid);

    for(size_t i = 0; i < rs.size(); i++) {
        if(!rs[i].filename.empty()) 
           std::cout << "Filename: " << rs[i].filename << std::endl;
        
        if(!rs[i].pathname.empty()) 
            std::cout << "Path: " << rs[i].pathname << std::endl;
        
        std::cout << "Start: " << std::hex << rs[i].start << std::endl;
        std::cout << "End: " << std::hex << rs[i].end << std::endl;
        std::cout << "Permissions: " << rs[i].read << rs[i].write << rs[i].exec << rs[i].shared << std::endl;
        std::cout << "Offset: " << std::hex << rs[i].offset << std::endl;
        std::cout << "Device: " << std::dec << rs[i].deviceMajor << ":" << rs[i].deviceMinor << std::endl;
        std::cout << "INode: " << std::dec << rs[i].inodeFileNumber << std::endl;
        std::cout << "-------------------------------------" << std::endl;
    }

    return 0;
}
