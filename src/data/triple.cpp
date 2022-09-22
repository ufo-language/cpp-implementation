#include <list>
#include <queue>

#include "data/triple.h"
#include "expr/identifier.h"
#include "memory/gc.h"

namespace ufo {

    D_Triple* const D_Triple::EMPTY = D_Triple::create(nullptr, nullptr, nullptr, GC::GC_Permanent);

    D_Triple* D_Triple::create(E_Identifier* ident, Any* value, D_Triple* next, GC::Lifetime lifetime) {
        D_Triple* triple = new D_Triple(ident, value, next, lifetime);
        return triple;
    }

    D_Triple::D_Triple(E_Identifier* ident, Any* value, D_Triple* next, GC::Lifetime lifetime)
        : Any{lifetime}, _ident{ident}, _value{value}, _next{next} {
    }

    Any* D_Triple::lookup(E_Identifier* ident) {
    }

    void D_Triple::markChildren(std::queue<Any*>& markedObjects) {
        if (!isEmpty()) {
            markedObjects.push(_ident);
            markedObjects.push(_value);
            markedObjects.push(_next);
        }
    }

    void D_Triple::show(std::ostream& stream) {
        stream << "A-TRIPLE";
    }

}
