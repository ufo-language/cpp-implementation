#pragma once

#include "data/any.h"
#include "ufo/typeid.h"

namespace ufo {

    class Evaluator;
    
    class D_Task : public Any {
    public:
        static D_Task* create(GC::Lifetime lifetime=GC::GC_Transient);

        // overridden methods
        Any* evaluate(Evaluator* etor) override;
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

        // unique methods

    protected:
        D_Task(GC::Lifetime lifetime)
            : Any{T_Task, lifetime} {
        }
    };

}
