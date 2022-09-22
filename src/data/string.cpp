#include "data/string.h"
#include "memory/gc.h"

namespace ufo {

    D_String* D_String::create(const std::string& s, GC::Lifetime lifetime) {
        return new D_String(s, lifetime);
    }

}
