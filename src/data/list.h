#pragma once

#include <iostream>
#include <queue>

#include "data/any.h"
#include "memory/gc.h"
#include "ufo/globals.h"
#include "ufo/typeid.h"
#include "ufo/ufoexception.h"

namespace ufo {

    class E_Continuation;
    class Evaluator;
    
    class D_List : public Any {
    public:
        static D_List* create(Any* first, Any* rest, GC::Lifetime lifetime=GC::GC_Transient);
        static D_List* create(std::initializer_list<Any*> elems, GC::Lifetime lifetime);

        // overridden methods
        void eval(Evaluator* etor) override;
        TypeId getTypeId() override { return T_List; }
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

        // unique methods
        virtual bool isEmpty() { return false; }
        virtual Any* getFirst() { return _first; }
        virtual Any* getRest() { return _rest; }
        virtual void setFirst(Any* first) { _first = first; }
        virtual void setRest(Any* rest) { _rest = rest; }

    protected:
        D_List(Any* first, Any* rest, GC::Lifetime lifetime)
           : Any{lifetime}, _first{first}, _rest{rest} {
        }

        Any* _first;
        Any* _rest;

    };

    class D_EmptyList : public D_List {
    public:
        bool isEmpty() override { return true; }

        Any* getFirst() override {
            throw UFOException("attempt to get 'first' field of empty list", this);
        }

        Any* getRest() override {
            throw UFOException("attempt to get 'rest' field of empty list", this);
        }

        void markChildren(std::queue<Any*>& markedObjects) override { (void)markedObjects; }

        void setFirst(Any* first) override {
            (void)first;
            throw UFOException("attempt to set 'first' field of empty list", this);
        }

        void setRest(Any* rest) override {
            (void)rest;
            throw UFOException("attempt to set 'rest' field of empty list", this);
        }

        friend Globals;

    protected:
        D_EmptyList()
            : D_List(nullptr, nullptr, GC::GC_Permanent) {}
    };

}
