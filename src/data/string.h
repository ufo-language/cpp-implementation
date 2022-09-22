#pragma once

#include <iostream>

#include "data/any.h"
#include "memory/gc.h"
#include "ufo/typeid.h"

namespace ufo {

    class D_String : public Any {
    public:
        static D_String* create(const std::string& s, GC::Lifetime lifetime=GC::GC_Transient);

        // overridden methods
        TypeId getTypeId() override { return T_String; }
        void show(std::ostream& stream) override { stream << _s; }

        // unique methods
        std::string getString() { return _s; }

    protected:
        D_String(const std::string s, GC::Lifetime lifetime)
            : Any{lifetime}, _s{s} {
        }

        const std::string _s;

    };

}
