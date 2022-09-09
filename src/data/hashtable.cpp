#include <iostream>
#include <unordered_map>

#include "data/hashtable.h"

namespace ufo {

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
