#include "data/any.h"
#include "etor/evaluator.h"

namespace ufo {

    Evaluator* Evaluator::create(GC::Lifetime lifetime) {
        return new Evaluator(lifetime);
    }

    Evaluator::Evaluator(GC::Lifetime lifetime)
        : Any(lifetime) {
    }

    Evaluator::~Evaluator() {
    }

    Any* Evaluator::lookup(E_Identifier* ident) {
        return _env->lookup(ident);
    }

    void Evaluator::markChildren(std::queue<Any*>& markedObjects) {
        markedObjects.push(_env);
    }


    Any* Evaluator::popExpr() {
        std::cerr << "Evaluator::popExpr is not implemented\n";
        return nullptr;
    }
    
    Any* Evaluator::popObj() {
        Any* top = _objStack.back();
        _objStack.pop_back();
        return top;
    }

    void Evaluator::pushExpr(Any* expr) {
        std::cerr << "Evaluator::pushExpr is not implemented\n";
    }

    void Evaluator::pushObj(Any* obj) {
        _objStack.push_back(obj);
    }

    void Evaluator::run() {
        std::cerr << "Evaluator::run is not implemented\n";
    }

    void Evaluator::step() {
        std::cerr << "Evaluator::step is not implemented\n";
    }

    void Evaluator::show(std::ostream& stream) {
        stream << "Evaluator{env=" << _env << "}\n";
    }

    void Evaluator::terminate() {
        _terminate = true;
    }

}
