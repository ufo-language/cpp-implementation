#pragma once

#include <iostream>

#include "data/any.h"
#include "gc/gc.h"
#include "ufo/globals.h"
#include "ufo/typeid.h"

namespace ufo {

    class D_Nil : public Any {
    public:
        // overridden methods
        void show(std::ostream& stream) override { stream << "nil"; }

    protected:
        D_Nil()
            : Any{T_Nil, GC::GC_Permanent} {
        }

    friend Globals;

    };

}
