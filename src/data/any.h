#pragma once

#include <iostream>
#include <queue>

#include "gc/gc.h"
#include "ufo/typeid.h"

namespace ufo {

    class Evaluator;

    class Any {
    public:
        Any(TypeId typeId, GC::Lifetime lifetime);
        virtual ~Any();

        // pure virtual functions
        TypeId getTypeId() { return _typeId; }
        virtual void show(std::ostream& stream) = 0;

        // virtual functions
        virtual void dispose() { delete this; }
        virtual Any* evaluate(Evaluator* etor) { (void)etor; return this; }
        virtual void markChildren(std::queue<Any*>& markedObjects) { (void)markedObjects; }
        virtual size_t size() { return sizeof(this); }

        // concrete functions
        bool isMarked() { return _isMarked; }
        Any* getNext() { return _next; }
        void setMarked(bool isMarked) { _isMarked = isMarked; }
        void setNext(Any* next) { _next = next; }

        // friend functions
        friend std::ostream & operator<<(std::ostream &stream, Any* object);

    protected:
        Any* _next;
        bool _isMarked = false;
        TypeId _typeId;

    };

    std::ostream& operator << (std::ostream& stream, std::nullptr_t ptr);

}
