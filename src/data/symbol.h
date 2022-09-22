#pragma once

#include <iostream>

#include "data/any.h"
#include "memory/gc.h"
#include "ufo/typeid.h"

namespace ufo {

    class D_Symbol : public Any {
    public:
        static D_Symbol* create(const std::string& name, GC::Lifetime lifetime=GC::GC_Transient);

        // overridden methods
        TypeId getTypeId() override { return T_Symbol; }
        void show(std::ostream& stream) override { stream << _name; }

        // unique methods

    protected:
        D_Symbol(const std::string& name, GC::Lifetime lifetime)
            : Any{lifetime}, _name{name} {
        }

        const std::string _name;
        unsigned int _hashCode;

    };

}
