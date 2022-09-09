#pragma once

#include <iostream>

#include "data/any.h"
#include "ufo/typeid.h"

namespace ufo {

    class D_Nil : public Any {
    public:
        D_Nil() {}

        TypeId getTypeId() override { return T_Nil; }

        void show(std::ostream& stream) { stream << "nil"; }

    protected:

    };

    extern D_Nil _NIL;
    extern D_Nil* NIL;

}
