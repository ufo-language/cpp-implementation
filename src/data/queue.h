#pragma once

#include <list>
#include <queue>

#include "data/any.h"
#include "data/list.h"
#include "gc/gc.h"
#include "ufo/globals.h"
#include "ufo/typeid.h"

namespace ufo {

    class Evaluator;
    
    class D_Queue : public Any {
    public:
        static D_Queue* create(const std::list<Any*>& elems={}, GC::Lifetime lifetime=GC::GC_Transient);

        // overridden methods
        Any* evaluate(Evaluator* etor) override;
        TypeId getTypeId() override { return T_Queue; }
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

        // unique methods
        D_List* asList() { return _elems; }
        int count() { return _count; }
        Any* deq();
        void enq(Any* object);
        bool isEmpty() { return _elems->isEmpty(); }

    protected:
        D_Queue(GC::Lifetime lifetime)
            : Any{lifetime}, _elems{GLOBALS.emptyList()}, _last{GLOBALS.emptyList()} {
        }

        D_List* _elems;
        D_List* _last;
        int _count = 0;
    };

}
