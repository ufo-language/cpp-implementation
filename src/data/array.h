#pragma once

#include <initializer_list>
#include <iostream>

#include "data/any.h"
#include "memory/gc.h"
#include "ufo/globals.h"
#include "ufo/typeid.h"
#include "ufo/ufoexception.h"

namespace ufo {

    class E_Continuation;
    class Evaluator;
    
    class D_Array : public Any {
    public:
        static D_Array* create(int count, GC::Lifetime lifetime=GC::GC_Transient);
        static D_Array* create(const std::initializer_list<Any*>& elems, GC::Lifetime lifetime=GC::GC_Transient);

        // overridden methods
        void eval(Evaluator* etor) override;
        TypeId getTypeId() override { return T_Array; }
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

        // unique methods
        virtual int count() { return _count; }

    protected:
        D_Array(int count, GC::Lifetime lifetime=GC::GC_Transient);
        D_Array(const std::initializer_list<Any*>& elems, GC::Lifetime lifetime=GC::GC_Transient);

        int _count;
        Any** _elems;

    };

}
