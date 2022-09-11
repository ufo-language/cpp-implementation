#pragma once

#include <iostream>
#include <queue>

#include "data/any.h"
#include "gc/gc.h"
#include "ufo/globals.h"
#include "ufo/typeid.h"
#include "ufo/ufoexception.h"

namespace ufo {

    class D_EmptyList;
    class Evaluator;
    
    class D_List : public Any {
    public:
        static D_List* create(Any* first, Any* rest, GC::Lifetime lifetime=GC::GC_Transient);

        // overridden methods
        void dispose() override;
        Any* evaluate(Evaluator* etor) override;
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
           : Any{T_List, lifetime}, _first{first}, _rest{rest} {
        }

        Any* _first;
        Any* _rest;

    };

    class D_EmptyList : public D_List {
    public:
        bool isEmpty() override { return true; }

        virtual Any* getFirst() {
            throw UFOException("attempt to get 'first' field of empty list", this);
        }
        virtual Any* getRest() {
            throw UFOException("attempt to get 'rest' field of empty list", this);
        }
        void markChildren(std::queue<Any*>& markedObjects) override { (void)markedObjects; }
        virtual void setFirst(Any* first) {
            (void)first;
            throw UFOException("attempt to set 'first' field of empty list", this);
        }
        virtual void setRest(Any* rest) {
            (void)rest;
            throw UFOException("attempt to set 'rest' field of empty list", this);
        }

        friend Globals;

    protected:
        D_EmptyList()
            : D_List(nullptr, nullptr, GC::GC_Permanent) {}
    };

}
