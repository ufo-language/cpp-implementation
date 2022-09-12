#include "data/integer.h"
#include "memory/gc.h"

namespace ufo {

    D_Integer* D_Integer::create(int n, GC::Lifetime lifetime) {
        return new D_Integer(n, lifetime);
    }

}
