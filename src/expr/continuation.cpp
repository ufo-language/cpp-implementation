#include "expr/continuation.h"
#include "etor/evaluator.h"

namespace ufo {

    E_Continuation* E_Continuation::create(const std::string& name, void (*fun)(E_Continuation* thisContin, Evaluator* etor), Any* arg) {
        return new E_Continuation(name, fun, arg);
    }

    void E_Continuation::eval(Evaluator* etor) {
        _fun(this, etor);
    }

    void E_Continuation::markChildren(std::queue<Any*>& markedObjects) {
        markedObjects.push(_arg);
    }

    void E_Continuation::show(std::ostream& stream) {
        stream << "E_Continuation{" << _name << ", " << (void*)_fun << ", " << _arg << "}";
    }

}
