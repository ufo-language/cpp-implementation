#include <iostream>

#include "data/list.h"
#include "memory/gc.h"

namespace ufo {

    D_List* D_List::create(Any* first, Any* rest, GC::Lifetime lifetime) {
        return new D_List(first, rest, lifetime);
    }
    
    void D_List::dispose() {
        // TODO recycle this object
        delete this;
    }

    Any* D_List::evaluate(Evaluator* etor) {
        return new D_List(_first->evaluate(etor), _rest->evaluate(etor), GC::GC_Transient);
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
