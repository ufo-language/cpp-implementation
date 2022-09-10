#pragma once

#include <queue>

#include "data/any.h"
#include "data/list.h"
#include "ufo/typeid.h"

namespace ufo {

    class Evaluator;
    
    class D_Queue : public Any {
    public:
        D_Queue()
            : Any{T_Queue}, _elems{EMPTY_LIST}, _last{EMPTY_LIST} {
        }

        // overridden methods
        Any* evaluate(Evaluator* etor) override;
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

        // unique methods
        D_List* asList() { return _elems; }
        int count() { return _count; }
        Any* deq();
        void enq(Any* object);
        bool isEmpty() { return _elems->isEmpty(); }

    protected:
        D_List* _elems;
        D_List* _last;
        int _count = 0;
    };

}
