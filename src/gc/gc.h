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
        void commit();
        bool isGCNeeded();

        // these functions should be protected, but they're public for testing
        void deleteAll();
        void dispose(std::queue<Any*>& deadObjects);
        bool isRegistered(Any* object);
        bool isRoot(Any* object);
        void mark();
        void sweep(std::queue<Any*>& deadObjects);

        void dump();

        unsigned int objectCountTrigger = 128;
        size_t objectResidencyTrigger = 4096;

    protected:
        Any* _allObjects;
        Any* _newObjects;
        std::vector<Any*> _permanentObjects;
        std::vector<Any*> _rootObjects;
        unsigned int _objectCount;
        size_t _objectResidency;
    };

    extern GC THE_GC;
    
}
