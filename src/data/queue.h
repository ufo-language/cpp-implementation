#pragma once

#include <iostream>
#include <queue>

#include "data/any.h"
#include "data/list.h"

namespace ufo {

    class Evaluator;
    
    class D_Queue : public Any {
    public:
        D_Queue()
            : _elems{EMPTY_LIST}, _last{EMPTY_LIST} {
        }

        TypeId getTypeId() override { return T_Queue; }

        Any* evaluate(Evaluator* etor) override;
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

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
