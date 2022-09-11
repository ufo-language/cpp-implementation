#include <iostream>
#include <unordered_map>

#include "data/hashtable.h"
#include "gc/gc.h"

#include <cassert>
namespace ufo {

    D_HashTable* create(GC::Lifetime lifetime) {
        (void)lifetime;
        assert(false);
    }

    void D_HashTable::dispose() {
    }

    Any* D_HashTable::evaluate(Evaluator* etor) {
        (void)etor;
        return this;
    }

    void D_HashTable::markChildren(std::queue<Any*>& markedObjects) {
        (void)markedObjects;
    }

    void D_HashTable::show(std::ostream& stream) {
        stream << "A-HASHTABLE";
    }

}
