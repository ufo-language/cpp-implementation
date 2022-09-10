#pragma once

#include <unordered_map>

#include "data/any.h"
#include "ufo/typeid.h"

namespace ufo {

    class Evaluator;
    
    class D_HashTable : public Any {
    public:
        D_HashTable()
            : Any{T_HashTable} {
        }

        // overridden methods
        void dispose() override;
        Any* evaluate(Evaluator* etor) override;
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

        // unique methods
        bool isEmpty() { return _hash.empty(); }

    protected:
        std::unordered_map<Any*, Any*> _hash;
    };

}
