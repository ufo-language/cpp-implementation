#pragma once

#include <iostream>

#include "data/any.h"
#include "memory/gc.h"
#include "ufo/typeid.h"

namespace ufo {

    class Evaluator;
    
    class E_Identifier : public Any {
    public:
        static E_Identifier* create(const std::string& name);

        // overridden methods
        void eval(Evaluator* etor) override;
        TypeId getTypeId() override { return T_Continuation; }
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

        // unique methods

    protected:
        E_Identifier(const std::string& name)
           : Any{GC::GC_Transient}, _name{name} {
        }

        const std::string _name;
    };

}
