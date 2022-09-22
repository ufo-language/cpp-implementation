#include "data/real.h"
#include "memory/gc.h"

namespace ufo {

    D_Real* D_Real::create(double r, GC::Lifetime lifetime) {
        return new D_Real(r, lifetime);
    }

}
