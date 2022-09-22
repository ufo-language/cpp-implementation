#include <list>
#include <queue>

#include "data/queue.h"
#include "etor/evaluator.h"
#include "memory/gc.h"

namespace ufo {

    D_Queue* D_Queue::create(GC::Lifetime lifetime) {
        return new D_Queue(lifetime);
    }

    D_Queue* D_Queue::create(std::initializer_list<Any*>& elems, GC::Lifetime lifetime) {
        D_Queue* queue = create(lifetime);
        for (Any* elem : elems) {
            queue->enq(elem);
        }
        return queue;
    }

    Any* D_Queue::deq() {
        if (_elems->isEmpty()) {
            throw UFOException("queue empty", this);
        }
        Any* elem = _elems->getFirst();
        _elems = (D_List*)_elems->getRest();
        _count--;
        return elem;
    }

    void D_Queue::enq(Any* elem) {
        if (_last->isEmpty()) {
            _elems = _last = D_List::create(elem, GLOBALS.emptyList());
        }
        else {
            D_List* last = D_List::create(elem, GLOBALS.emptyList());
            _last->setRest(last);
            _last = last;
        }
        _count++;
    }

    static void _contin(E_Continuation* thisContin, Evaluator* etor) {
        (void)thisContin;
        (void)etor;
        std::cout << "D_Queue::_contin is not implemented\n";
    }

    void D_Queue::eval(Evaluator* etor) {
        (void)etor;
        std::cout << "D_Queue::eval is not implemented\n";
#if 0
        D_Queue* qNew = new D_Queue(GC::GC_Transient);
        D_List* elems = _elems;
        while (!elems->isEmpty()) {
            Any* elem = _elems->getFirst();
            Any* elem1 = etor->evaluate(elem);
            qNew->enq(elem1);
            elems = (D_List*)elems->getRest();
        }
#endif
    }

    void D_Queue::markChildren(std::queue<Any*>& markedObjects) {
        D_List* elems = _elems;
        while (!elems->isEmpty()) {
            Any* elem = elems->getFirst();
            markedObjects.push(elem);
            elems = (D_List*)elems->getRest();
        }
    }

    void D_Queue::show(std::ostream& stream) {
        stream << "~";
        _elems->show(stream);
    }

}
