#pragma once

#include <iostream>
#include <queue>

#include "gc/gc.h"
#include "ufo/typeid.h"

namespace ufo {

    class Evaluator;

    class Any {
    public:
        Any();
        virtual ~Any();

        virtual TypeId getTypeId() = 0;
        virtual void markChildren(std::queue<Any*>& markedObjects) = 0;
        virtual void show(std::ostream& stream) = 0;

        virtual void dispose() { delete this; }
        virtual Any* evaluate(Evaluator* etor) { (void)etor; return this; }

        bool isMarked() { return _isMarked; }
        Any* getNext() { return _next; }
        void setMarked(bool isMarked) { _isMarked = isMarked; }
        void setNext(Any* next) { _next = next; }

        friend std::ostream & operator<<(std::ostream &stream, Any* object);

    protected:
        Any* _next;
        bool _isMarked = false;

    };

    std::ostream& operator << (std::ostream& stream, std::nullptr_t ptr);

}
