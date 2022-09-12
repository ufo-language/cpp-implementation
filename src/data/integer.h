#pragma once

#include <iostream>

#include "data/any.h"
#include "memory/gc.h"
#include "ufo/typeid.h"

namespace ufo {

    class D_Integer : public Any {
    public:
        static D_Integer* create(int n, GC::Lifetime lifetime=GC::GC_Transient);

        // overridden methods
        TypeId getTypeId() override { return T_Integer; }
        void show(std::ostream& stream) override { stream << _n; }

        // unique methods
        int getValue() { return _n; }

    protected:
        D_Integer(int n, GC::Lifetime lifetime)
            : Any{lifetime}, _n{n} {
        }

        int _n;

    };

}
