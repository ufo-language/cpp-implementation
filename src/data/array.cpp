#include <iostream>

#include "data/array.h"
#include "etor/evaluator.h"
#include "expr/continuation.h"
#include "memory/gc.h"

namespace ufo {

    D_Array* D_Array::create(int count, GC::Lifetime lifetime) {
        return new D_Array(count, lifetime);
    }

    D_Array* D_Array::create(const std::initializer_list<Any*>& elems, GC::Lifetime lifetime) {
        return new D_Array(elems, lifetime);
    }

    D_Array::D_Array(int count, GC::Lifetime lifetime)
        : Any{lifetime} {
        // TODO
        (void)count;
        std::cerr << "D_Array::D_Array is incomplete\n";
    }

    D_Array::D_Array(const std::initializer_list<Any*>& elems, GC::Lifetime lifetime)
        : Any{lifetime} {
        // TODO
        (void)elems;
        std::cerr << "D_Array::D_Array is incomplete\n";
    }

#if 0
    static void _contin(E_Continuation* thisContin, Evaluator* etor) {
        (void)thisContin;
        Any* first = etor->popObj();
        Any* rest = etor->popObj();
        D_List* list = D_List::create(first, rest);
        etor->pushObj(list);
    }

    void D_List::eval(Evaluator* etor) {
        etor->pushExpr(E_Continuation::create("list", _contin, (Any*)GLOBALS.nil()));
        etor->pushExpr(_first);
        etor->pushExpr(_rest);
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
            if (rest->isA(T_List)) {
                list = (D_List*)rest;
            }
            else {
                stream << " | " << rest;
                break;
            }
        }
        stream << ']';
    }
#endif
}
