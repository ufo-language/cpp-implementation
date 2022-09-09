#include <iostream>

#include "console.h"

namespace ufo {

    Console::Console() {
    }

    Console::~Console() {
    }

    int Console::run() {
        std::cout << "Console::run called\n";
        return 0;
    }

}
