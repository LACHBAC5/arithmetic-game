#include "read.hpp"

namespace lb{
    std::string File::readFile(const std::string& name){
        std::string out;
        int bytes=0; char buffer[64];
        int fd = open(name.c_str(), 0);
        while((bytes=read(fd, buffer, sizeof(buffer)/sizeof(char)-1))>0){
            buffer[bytes]=0;
            out += buffer;
        }
        close(fd);
        return out;
    }
}