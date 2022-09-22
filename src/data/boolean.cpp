#include "data/boolean.h"
#include "memory/gc.h"

namespace ufo {

    D_Boolean* D_Boolean::create(bool b, GC::Lifetime lifetime) {
        return new D_Boolean(b, lifetime);
    }

}
