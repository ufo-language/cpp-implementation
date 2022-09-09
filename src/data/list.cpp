#include <iostream>

#include "list.h"

namespace ufo {

    D_EmptyList _EMPTY_LIST;
    D_List* EMPTY_LIST = &_EMPTY_LIST;

    void D_List::dispose() {
    }

    Any* D_List::evaluate(Evaluator* etor) {
        return new D_List(_first->evaluate(etor), _rest->evaluate(etor));
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
