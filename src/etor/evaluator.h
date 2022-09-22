#pragma once

#include <vector>

#include "data/any.h"
#include "data/list.h"
#include "data/triple.h"
#include "memory/gc.h"

namespace ufo {

    class TaskManager;

    class Evaluator : public Any {
    public:
        static Evaluator* create(GC::Lifetime lifetime=GC::GC_Transient);

        ~Evaluator();

        // overridden methods
        TypeId getTypeId() override { return T_Evaluator; }
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;
        
        // unique methods
        void bind(E_Identifier* ident, Any* value) { _env = D_Triple::create(ident, value, _env); }
        D_Triple* getEnv() { return _env; }
        Any* lookup(E_Identifier* ident);
        Any* popExpr();
        Any* popObj();
        void pushExpr(Any* expr);
        void pushObj(Any* obj);
        void run();
        void terminate();

        friend TaskManager;

    protected:
        Evaluator(GC::Lifetime lifetime=GC::GC_Transient);

        void step();

        std::vector<Any*> _objStack;
        std::vector<Any*> _exprStack;
        std::vector<Any*> _envStack;
        D_Triple* _env = GLOBALS.emptyTriple();
        bool _terminate = false;
        bool _isRunning = false;

        Evaluator* _nextEvaluator;

    };

}
