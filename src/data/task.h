#include "data/any.h"
#include "etor/evaluator.h"
#include "ufo/typeid.h"

namespace ufo {

    class D_Task : public Any {
    public:
        static D_Task* create(Evaluator* etor);

        // overridden methods
        TypeId getTypeId() override { return T_Task; }
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

        // unique methods

    protected:
        D_Task(Evaluator* etor, GC::Lifetime lifetime=GC::GC_Transient)
            : Any{lifetime}, _etor{etor} {
        }

        Evaluator* _etor;
    };

}
