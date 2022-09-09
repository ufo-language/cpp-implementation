#pragma once

#include <unordered_map>

#include "data/any.h"
#include "ufo/typeid.h"

namespace ufo {

    class Evaluator;
    
    class D_HashTable : public Any {

    public:
        D_HashTable() {}

        TypeId getTypeId() override { return T_HashTable; }

        void dispose() override;
        Any* evaluate(Evaluator* etor) override;
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

        bool isEmpty() { return _hash.empty(); }

    protected:
        std::unordered_map<Any*, Any*> _hash;
    };

}
