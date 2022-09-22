#include "data/symbol.h"
#include "memory/gc.h"

namespace ufo {

    D_Symbol* D_Symbol::create(const std::string& name, GC::Lifetime lifetime) {
        return new D_Symbol(name, lifetime);
    }

}
