#include <iostream>

#include "ufo/console.h"

int ARGC;
char** ARGV;

int main(int argc, char* argv[]) {
    ARGC = argc;
    ARGV = argv;
    std::cout << "main starting\n";
    ufo::Console console;
    int resultCode = console.run();
    std::cout << "main ending\n";
    return resultCode;
}
