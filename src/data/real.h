#pragma once

#include <iostream>

#include "data/any.h"
#include "memory/gc.h"
#include "ufo/typeid.h"

namespace ufo {

    class D_Real : public Any {
    public:
        static D_Real* create(double r, GC::Lifetime lifetime=GC::GC_Transient);

        // overridden methods
        TypeId getTypeId() override { return T_Real; }
        void show(std::ostream& stream) override { stream << _r; }

        // unique methods
        double getValue() { return _r; }

    protected:
        D_Real(double r, GC::Lifetime lifetime)
            : Any{lifetime}, _r{r} {
        }

        double _r;

    };

}
