#pragma once

#include <iostream>

#include "data/any.h"
#include "ufo/typeid.h"

namespace ufo {

    class D_Integer : public Any {
    public:
        D_Integer(int n)
            : Any{T_Integer}, _n{n} {
        }

        // overridden methods
        void show(std::ostream& stream) override { stream << _n; }

        // unique methods
        int getValue() { return _n; }

    protected:
        int _n;

    };

}
