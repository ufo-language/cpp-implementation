#pragma once

#include <iostream>

#include "data/any.h"
#include "ufo/typeid.h"

namespace ufo {

    class D_Integer : public Any {
    public:
        D_Integer(int n)
            : Any{} {
            _n = n;
        }

        // overridden methods
        TypeId getTypeId() override { return T_Integer; }

        // unique methods
        int getValue() { return _n; }
        void show(std::ostream& stream) { stream << _n; }

    protected:
        int _n;

    };

}
