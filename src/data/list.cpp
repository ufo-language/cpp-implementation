#include <iostream>

#include "list.h"

namespace ufo {

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
        stream << "A-LIST";
    }

}
