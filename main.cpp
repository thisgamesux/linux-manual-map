#include "main.hpp"

using namespace std;

int main() {

    remote::Handle proc = remote::OpenProcess(5388);

    if(proc.IsRunning()) {
        remote::Module libc = remote::GetModuleHandle(proc, "libpthread-2.19.so");

        if(libc.IsValid()) {
            // We need to grab symbol list
            cout << "libc = " << hex << libc.GetStart() << endl;
        }

        /*
        std::vector<remote::Region> rs = remote::GetRegions(proc.pid);

        for(size_t i = 0; i < rs.size(); i++) {
            if(!rs[i].filename.empty()) {
                std::cout << "Filename: " << rs[i].filename << std::endl;
            }

            if(!rs[i].pathname.empty()) {
                std::cout << "Path: " << rs[i].pathname << std::endl;
            }

            std::cout << "Start: " << std::hex << rs[i].start << std::endl;
            std::cout << "End: " << std::hex << rs[i].end << std::endl;
            std::cout << "Permissions: " << rs[i].read << rs[i].write << rs[i].exec << rs[i].shared << std::endl;
            std::cout << "Offset: " << std::hex << rs[i].offset << std::endl;
            std::cout << "Device: " << std::dec << rs[i].deviceMajor << ":" << rs[i].deviceMinor << std::endl;
            std::cout << "INode: " << std::dec << rs[i].inodeFileNumber << std::endl;
            std::cout << "-------------------------------------" << std::endl;
        }

        size_t calc_base = proc.GetModuleAddress("gnome-calculator");

        cout << "Calculator Base: " << hex << calc_base << endl;
        */
    }

    return 0;
}