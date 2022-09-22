#pragma once

#include <iostream>

#include "data/any.h"
#include "memory/gc.h"
#include "ufo/typeid.h"

namespace ufo {

    class D_Boolean : public Any {
    public:
        static D_Boolean* create(bool b, GC::Lifetime lifetime=GC::GC_Transient);

        // overridden methods
        TypeId getTypeId() override { return T_Boolean; }
        void show(std::ostream& stream) override { stream << (_b ? "true" : "false"); }

        // unique methods
        int getValue() { return _b; }

    protected:
        D_Boolean(bool b, GC::Lifetime lifetime)
            : Any{lifetime}, _b{b} {
        }

        bool _b;

    };

}
