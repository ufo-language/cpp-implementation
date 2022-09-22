#include <iostream>
#include <unordered_map>

#include "data/hashtable.h"
#include "etor/evaluator.h"
#include "memory/gc.h"
#include "ufo/globals.h"

#include <cassert>
namespace ufo {

    D_HashTable* create(GC::Lifetime lifetime) {
        (void)lifetime;
        assert(false);
    }

    void D_HashTable::dispose() {
        std::cerr << "D_HashTable::dispose is incomplete\n";
    }

    void D_HashTable::eval(Evaluator* etor) {
        (void)etor;
        std::cerr << "D_HashTable::evaluate is incomplete\n";
        etor->pushObj((Any*)GLOBALS.nil());
    }

    void D_HashTable::markChildren(std::queue<Any*>& markedObjects) {
        (void)markedObjects;
        std::cerr << "D_HashTable::markChildren is incomplete\n";
    }

    void D_HashTable::show(std::ostream& stream) {
        stream << "A-HASHTABLE";
    }

}
