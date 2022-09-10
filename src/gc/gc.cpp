#include <queue>

#include "data/any.h"
#include "gc/gc.h"

namespace ufo {

    GC THE_GC;

    GC::GC() {}

    GC::~GC() {
        deleteAll();
    }

    void GC::addRoot(Any* object) {
        _rootObjects.push_back(object);
    }

    void GC::addObject(Any* object) {
        object->setNext(_spine);
        _spine = object;
        _objectCount++;
        _objectResidency += object->size();
    }

    void GC::collect() {
        mark();
        std::queue<Any*> deadObjects;
        sweep(deadObjects);
        dispose(deadObjects);
    }

    void GC::deleteAll() {
        while (_spine) {
            Any* next = _spine->getNext();
            _spine->dispose();
            _spine = next;
        }
        _spine = nullptr;
        _committedObjects = nullptr;
        _rootObjects.clear();
        _objectCount = 0;
        _objectResidency = 0;
    }

    void GC::dispose(std::queue<Any*>& deadObjects) {
        int nObjs = deadObjects.size();
        for (int n=0; n<nObjs; n++) {
            Any* object = deadObjects.front();
            deadObjects.pop();
            _objectCount--;
            _objectResidency -= object->size();
            object->dispose();
        }
    }

    void GC::dump() {
        Any* object = _spine;
        int n = 0;
        std::cerr << "GC dump:\n";
        bool committed = false;
        while (object) {
            if (object == _committedObjects) {
                committed = true;
            }
            std::cerr << n++ << ". ";
            std::cerr << "Root:" << (isRoot(object) ? "[+] " : "[_] ");
            std::cerr << "Comm:" << (committed ? "[+] " : "[_] ");
            std::cerr << "Mark:" << (object->isMarked() ? "[+] " : "[_] ");
            std::cerr << object << " @" << (void*)object;
            std::cerr << " -> " << (void*)object->getNext();
            std::cerr << "\n";
            object = object->getNext();
        }
    }

    bool GC::isGCNeeded() {
        return _objectCount >= objectCountTrigger || _objectResidency >= objectResidencyTrigger;
    }

    bool GC::isCommitted(Any* object) {
        bool isCommitted = false;
        Any* objects = _spine;
        while (objects) {
            if (objects == _committedObjects) {
                isCommitted = true;
            }
            if (objects == object) {
                return isCommitted;
            }
            objects = objects->getNext();
        }
        return false;
    }

    bool GC::isRegistered(Any* object) {
        Any* objects = _spine;
        while (objects) {
            if (object == objects) {
                return true;
            }
            objects = objects->getNext();
        }
        return false;
    }

    bool GC::isRoot(Any* object) {
        for (Any* rootObject : _rootObjects) {
            if (object == rootObject) {
                return true;
            }
        }
        return false;
    }

    void GC::mark() {
        std::queue<Any*> markedObjects;
        // mark all root objects
        for (Any* object : _rootObjects) {
            markedObjects.push(object);
        }
        // mark all new objects
        Any* spine = _spine;
        while (spine != _committedObjects) {
            markedObjects.push(spine);
            spine = spine->getNext();
        }
        while (!markedObjects.empty()) {
            Any* object = markedObjects.front();
            markedObjects.pop();
            if (!object->isMarked()) {
                object->setMarked(true);
                object->markChildren(markedObjects);
            }
        }
    }

    void GC::sweep(std::queue<Any*>& /*out*/ deadObjects) {
        Any* prev = NULL;
        Any* object = _spine;
        // these are the non-committed objects
        // just unmark them but do not sweep them
        while (object != _committedObjects) {
            object->setMarked(false);
            prev = object;
            object = object->getNext();
        }
        // these are the committed objects
        // they may be swept
        while (object != _permanentObjects) {
            Any* next = object->getNext();
            if (object->isMarked()) {
                object->setMarked(false);
                prev = object;
            }
            else {
                // disconnect and delete the object
                if (prev == NULL) {
                    _spine = next;
                }
                else {
                    prev->setNext(next);
                }
                if (object == _committedObjects) {
                    _committedObjects = next;
                }
                deadObjects.push(object);
            }
            object = next;
        }
        // unmark all root ojects
        for (Any* object : _rootObjects) {
            object->setMarked(false);
        }
    }

}
