#pragma once

#include <iostream>
#include <queue>

#include "memory/gc.h"
#include "ufo/typeid.h"

namespace ufo {

    class Evaluator;

    class Any {
    public:
        Any(GC::Lifetime lifetime);
        virtual ~Any();

        // pure virtual functions
        virtual TypeId getTypeId() = 0;
        virtual void show(std::ostream& stream) = 0;

        // virtual functions
        virtual void dispose() { delete this; }
        virtual void eval(Evaluator* etor);
        virtual void markChildren(std::queue<Any*>& markedObjects) { (void)markedObjects; }
        virtual size_t size() { return sizeof(this); }

        // concrete functions
        bool isA(TypeId typeId) { return getTypeId() == typeId; }
        bool isMarked() { return _isMarked; }
        Any* getNext() { return _next; }
        void setMarked(bool isMarked) { _isMarked = isMarked; }
        void setNext(Any* next) { _next = next; }

        // friend functions
        friend Evaluator;
        friend std::ostream & operator<<(std::ostream &stream, Any* object);

    protected:
        Any* _next;
        bool _isMarked = false;
    };

    std::ostream& operator << (std::ostream& stream, std::nullptr_t ptr);

}
