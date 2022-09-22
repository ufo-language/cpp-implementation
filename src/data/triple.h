#pragma once

#include <queue>

#include "data/any.h"
#include "memory/gc.h"
#include "ufo/globals.h"
#include "ufo/typeid.h"

namespace ufo {

    class E_Identifier;
    class Evaluator;
    
    class D_Triple : public Any {
    public:
        static D_Triple* const EMPTY;
        static D_Triple* create(E_Identifier* ident, Any* value, D_Triple* next, GC::Lifetime lifetime=GC::GC_Transient);

        // overridden methods
        TypeId getTypeId() override { return T_Triple; }
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

        // unique methods
        E_Identifier* getIdent() { return _ident; }
        Any* getValue() { return _value; }
        D_Triple* getNext() { return _next; }
        bool isEmpty() { return this == D_Triple::EMPTY; }
        Any* lookup(E_Identifier* ident);

        friend Globals;

    protected:
        D_Triple(E_Identifier* ident, Any* value, D_Triple* next, GC::Lifetime lifetime);

        E_Identifier* _ident;
        Any* _value;
        D_Triple* _next;

    };

}
