#include "data/integer.h"
#include "memory/gc.h"

namespace ufo {

    D_Integer* D_Integer::create(int i, GC::Lifetime lifetime) {
        return new D_Integer(i, lifetime);
    }

}
