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

        // should be protected, but they're public for testing
        void deleteAll();
        void dispose(std::queue<Any*>& deadObjects);
        bool isRegistered(Any* object);
        bool isRoot(Any* object);
        void mark();
        void sweep(std::queue<Any*>& deadObjects);

        void dump();

    protected:
        Any* _allObjects;
        std::vector<Any*> _permantentObjects;
        std::vector<Any*> _rootObjects;
        unsigned int _numObjects;
    };

    extern GC THE_GC;
    
}
