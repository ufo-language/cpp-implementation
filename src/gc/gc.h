#pragma once

#include <vector>

namespace ufo {

    class Any;
    
    class GC {
    public:
        GC();
        ~GC();

        void addObject(Any* object);
        void addRoot(Any* object);
        void collect();
        void commit() { _committedObjects = _spine; }
        bool isGCNeeded();

        // these functions should be protected, but they're public for testing
        void deleteAll();
        void dispose(std::queue<Any*>& deadObjects);
        bool isCommitted(Any* object);
        bool isRegistered(Any* object);
        bool isRoot(Any* object);
        void mark();
        void sweep(std::queue<Any*>& deadObjects);

        void dump();

        unsigned int objectCountTrigger = 128;
        size_t objectResidencyTrigger = 4096;

    protected:
        Any* _spine = nullptr;  // all non-commited objects; tail ponts to _committedObjects
        Any* _committedObjects = nullptr;
        Any* _permanentObjects = nullptr;
        unsigned int _objectCount = 0;
        size_t _objectResidency = 0;
        std::vector<Any*> _rootObjects;
    };

    extern GC THE_GC;
    
}
