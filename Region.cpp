#include "Region.hpp"

using namespace std;
using namespace remote;

vector<Region> remote::GetRegions(pid_t pid) {
    vector<Region> ret;

    stringstream path;
    path << "/proc/" << pid << "/maps";

    ifstream m(path.str().c_str());

    string line;
    while(getline(m, line)) {
        Region r;

        size_t count = 0;

        string str = line;

        while(str.length()) {
            size_t nextSpace = str.find_first_of(' ');

            if(nextSpace == -1) {
                if(!str.empty()) {
                    r.pathname = str;

                    if(str.c_str()[0] == '/') {
                        r.filename = str.substr(str.find_last_of('/') + 1, str.length());
                    } else {
                        r.filename = r.pathname;
                    }
                }

                break; // Stuff
            } else {
                string content = str.substr(0, nextSpace);

                // Deal with content
                stringstream ss;

                if(count == 0) {
                    size_t spl = content.find_first_of('-');

                    if(spl != -1) {
                        ss << hex << content.substr(0, spl);
                        ss >> r.start;
                        ss.clear();
                        ss << hex << content.substr(spl + 1, content.size());
                        ss >> r.end;
                        ss.clear();
                    }
                } else if(count == 1) { // Permissions
                    r.read = (content.c_str()[0] == 'r');
                    r.write = (content.c_str()[1] == 'w');
                    r.exec = (content.c_str()[2] == 'x');
                    r.shared = (content.c_str()[3] == 'p');
                } else if(count == 2) { // Offset
                    ss << hex << content;
                    ss >> r.offset;
                    ss.clear();
                } else if(count == 3) { // Device Info
                    size_t spl = content.find_first_of(':');

                    if(spl != -1) {
                        ss << dec << content.substr(0, spl);
                        ss >> r.deviceMajor;
                        ss.clear();
                        ss << dec << content.substr(spl + 1, content.size());
                        ss >> r.deviceMinor;
                        ss.clear();
                    }
                } else if(count == 4) { // INode shit
                    ss << content;
                    ss >> r.inodeFileNumber;
                    ss.clear();
                }

                count++;

                // Advance
                str = str.substr(nextSpace);

                size_t firstNonSpace = str.find_first_not_of(' ');

                if(firstNonSpace == -1) {
                    break;
                }

                str = str.substr(firstNonSpace);
            }
        }

        if(r.start != 0 && r.end != 0) {
            ret.push_back(r);
        }
    }

    return ret;
}