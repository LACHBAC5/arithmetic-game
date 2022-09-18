#pragma once

#include <iostream>
#include <fcntl.h>
#include <unistd.h>

namespace lb::File {
    std::string readFile(const std::string& name);
}
