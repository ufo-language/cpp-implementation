#pragma once

#include <iostream>

#include "data/any.h"
#include "memory/gc.h"
#include "ufo/globals.h"
#include "ufo/typeid.h"

namespace ufo {

    class E_Continuation;
    class Evaluator;
    
    class E_Continuation : public Any {
    public:
        static E_Continuation* create(const std::string& name, void (*fun)(E_Continuation* thisContin, Evaluator* etor), Any* arg);

        // overridden methods
        void eval(Evaluator* etor) override;
        TypeId getTypeId() override { return T_Continuation; }
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

        // unique methods

    protected:
        E_Continuation(const std::string& name, void (*fun)(E_Continuation* thisContin, Evaluator* etor), Any* arg)
           : Any{GC::GC_Transient}, _name{name}, _fun{fun}, _arg{arg} {
        }

        const std::string _name;
        void (*_fun)(E_Continuation* thisContin, Evaluator* etor);
        Any* _arg;
    };

}
