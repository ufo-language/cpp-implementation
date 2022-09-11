#pragma once

#include <vector>

namespace ufo {

    class Any;
    
    class GC {
    public:
        GC();
        ~GC();

        // Permanent objects are leaf objects that have no children to mark. The objects are never swept.
        // Root objects have children that are marked, but the objects are never swept.
        // Transient objects are all other objects.
        enum Lifetime {
            GC_Permanent, GC_Root, GC_Transient, GC_Unmanaged
        };

        void addObject(Any* object, Lifetime lifetime);
        void collect();
        void commit() { _committedObjects = _spine; }
        bool isGCNeeded();

        // these functions should be protected, but they're public for testing
        void deleteAll();
        void deletePermanentObjects();
        void dispose(std::queue<Any*>& deadObjects);
        void dump();
        bool isCommitted(Any* object);
        bool isPermanent(Any* object);
        int getNumPermanents() { return _nPermanentObjects; }
        bool isRegistered(Any* object);
        int getNumRegistered() { return _nRegisteredObjects; }
        bool isRoot(Any* object);
        int getNumRoots() { return _nRootObjects; }
        void mark();
        void sweep(std::queue<Any*>& deadObjects);

        unsigned int objectCountTrigger = 128;
        size_t memoryResidencyTrigger = 4096;

    protected:
        void _deleteList(Any* list);

        Any* _spine = nullptr;  // all non-commited objects
        int _nRegisteredObjects = 0;
        Any* _committedObjects = nullptr;
        Any* _rootObjects = nullptr;
        int _nRootObjects = 0;
        Any* _permanentObjects = nullptr;
        int _nPermanentObjects = 0;
        unsigned int _objectCount = 0;
        size_t _memoryResidency = 0;
    };

    extern GC THE_GC;
    
}
