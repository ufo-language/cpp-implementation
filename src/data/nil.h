#pragma once

#include <iostream>

#include "data/any.h"
#include "ufo/typeid.h"

namespace ufo {

    class D_Nil : public Any {
    public:
        D_Nil()
            : Any{T_Nil} {
        }

        // overridden methods
        void show(std::ostream& stream) override { stream << "nil"; }

    protected:

    };

    extern D_Nil _NIL;
    extern D_Nil* NIL;

}
