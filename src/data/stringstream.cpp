#include "data/stringstream.h"

namespace ufo {

    D_StringStream* D_StringStream::create(GC::Lifetime lifetime) {
        return new D_StringStream("", lifetime);
    }

    D_StringStream* D_StringStream::create(const std::string& string, GC::Lifetime lifetime) {
        return new D_StringStream(string, lifetime);
    }

    char D_StringStream::getNextChar() {
        std::cerr << "D_StringStream::getChar is incomplete\n";        
        return '\0';
    }

    void D_StringStream::getPos(int& pos, int& line, int& col) {
        (void)pos;
        (void)line;
        (void)col;
        std::cerr << "D_StringStream::getPos is incomplete\n";
    }

    D_StringStream::D_StringStream(const std::string& string, GC::Lifetime lifetime)
        : Any{lifetime} {
        // TODO
        (void)string;
        std::cerr << "D_StringStream::D_StringStream is incomplete\n";
    }

    void D_StringStream::show(std::ostream& stream) {
        stream << "A-STRINGSTREAM";
    }

}
