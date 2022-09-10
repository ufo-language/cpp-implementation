#pragma once

#include "data/any.h"
#include "ufo/typeid.h"

namespace ufo {

    class Evaluator;
    
    class D_Task : public Any {
    public:
        D_Task()
            : Any{T_Task} {
        }

        // overridden methods
        Any* evaluate(Evaluator* etor) override;
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

        // unique methods

    protected:

    };

}
