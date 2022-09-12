#pragma once

#include <unordered_map>

#include "data/any.h"
#include "memory/gc.h"
#include "ufo/typeid.h"

namespace ufo {

    class Evaluator;
    
    class D_HashTable : public Any {
    public:
        static D_HashTable* create(GC::Lifetime lifetime=GC::GC_Transient);

        // overridden methods
        void dispose() override;
        Any* evaluate(Evaluator* etor) override;
        TypeId getTypeId() override { return T_HashTable; }
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

        // unique methods
        bool isEmpty() { return _hash.empty(); }

    protected:
        D_HashTable(GC::Lifetime lifetime)
            : Any{lifetime} {
        }

        std::unordered_map<Any*, Any*> _hash;
    };

}
