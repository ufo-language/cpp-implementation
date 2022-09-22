#include <initializer_list>
#include <iostream>

#include "data/list.h"
#include "data/queue.h"
#include "memory/gc.h"

namespace ufo {

    D_List* D_List::create(Any* first, Any* rest, GC::Lifetime lifetime) {
        return new D_List(first, rest, lifetime);
    }
    
    D_List* D_List::create(std::initializer_list<Any*> elems, GC::Lifetime lifetime) {
        D_Queue* q = D_Queue::create(elems);
        return q->asList();
    }

#if 0
    void D_List::dispose() {
        // TODO recycle this object
        delete this;
    }
#endif

    void D_List::eval(Evaluator* etor) {
        //return new D_List(_first->evaluate(etor), _rest->evaluate(etor), GC::GC_Transient);
        std::cerr << "D_List::eval is not implemented\n";
    }

    void D_List::markChildren(std::queue<Any*>& markedObjects) {
        markedObjects.push(_first);
        markedObjects.push(_rest);
    }

    void D_List::show(std::ostream& stream) {
        D_List* list = this;
        bool firstIter = true;
        stream << '[';
        while (!list->isEmpty()) {
            if (firstIter) {
                firstIter = false;
            }
            else {
                stream << ", ";
            }
            stream << list->getFirst();
            Any* rest = list->getRest();
            if (rest->getTypeId() == T_List) {
                list = (D_List*)rest;
            }
            else {
                stream << " | " << rest;
                break;
            }
        }
        stream << ']';
    }

}
