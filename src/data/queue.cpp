#include <iostream>

#include "queue.h"

namespace ufo {

    void D_Queue::dispose() {
    }

    void D_Queue::enq(Any* elem) {
        if (_last->isEmpty()) {
            _elems = _last = new D_List(elem, EMPTY_LIST);
        }
        else {
            D_List* last = new D_List(elem, EMPTY_LIST);
            _last->setRest(last);
            _last = last;
        }
    }

    Any* D_Queue::evaluate(Evaluator* etor) {
        D_Queue* qNew = new D_Queue();
        D_List* elems = _elems;
        while (!elems->isEmpty()) {
            Any* elem = _elems->getFirst();
            Any* elem1 = elem->evaluate(etor);
            qNew->enq(elem1);
            elems = (D_List*)elems->getRest();
        }
        return qNew;
    }

    void D_Queue::markChildren(std::queue<Any*>& markedObjects) {
        D_List* elems = _elems;
        while (!elems->isEmpty()) {
            Any* elem = _elems->getFirst();
            markedObjects.push(elem);
            elems = (D_List*)elems->getRest();
        }
    }

    void D_Queue::show(std::ostream& stream) {
        stream << "A-QUEUE";
    }

}
