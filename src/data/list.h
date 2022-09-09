#pragma once

#include <iostream>
#include <queue>

#include "data/any.h"
#include "ufo/typeid.h"
#include "ufo/ufoexception.h"

namespace ufo {

    class D_EmptyList;
    class Evaluator;
    
    class D_List : public Any {
    public:
        D_List(Any* first, Any* rest)
           : Any{}, _first{first}, _rest{rest} {}

        TypeId getTypeId() override { return T_List; }

        void dispose() override;
        Any* evaluate(Evaluator* etor) override;
        void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

        virtual bool isEmpty() { return false; }
        virtual Any* getFirst() { return _first; }
        virtual Any* getRest() { return _rest; }
        virtual void setFirst(Any* first) { _first = first; }
        virtual void setRest(Any* rest) { _rest = rest; }

    protected:
        Any* _first;
        Any* _rest;

    };

    class D_EmptyList : public D_List {
    public:
        D_EmptyList()
            : D_List(nullptr, nullptr) {}

        bool isEmpty() override { return true; }

        virtual Any* getFirst() {
            throw UFOException("attempt to get 'first' field of empty list", this);
        }
        virtual Any* getRest() {
            throw UFOException("attempt to get 'rest' field of empty list", this);
        }
        virtual void setFirst(Any* first) {
            (void)first;
            throw UFOException("attempt to set 'first' field of empty list", this);
        }
        virtual void setRest(Any* rest) {
            (void)rest;
            throw UFOException("attempt to set 'rest' field of empty list", this);
        }

    };

    extern D_EmptyList _EMPTY_LIST;
    extern D_List* EMPTY_LIST;

}
