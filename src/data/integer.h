#pragma once

#include <iostream>

#include "data/any.h"
#include "gc/gc.h"
#include "ufo/typeid.h"

namespace ufo {

    class D_Integer : public Any {
    public:
        static D_Integer* create(int n, GC::Lifetime lifetime=GC::GC_Transient);

        // overridden methods
        void show(std::ostream& stream) override { stream << _n; }

        // unique methods
        int getValue() { return _n; }

    protected:
        D_Integer(int n, GC::Lifetime lifetime)
            : Any{T_Integer, lifetime}, _n{n} {
        }

        int _n;

    };

}
