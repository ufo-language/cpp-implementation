#include <queue>

#include "data/any.h"
#include "gc/gc.h"

namespace ufo {

    GC THE_GC;

    GC::GC()
        : _allObjects{nullptr}, _newObjects{nullptr}, _objectCount{0}, _objectResidency{0} {
    }

    GC::~GC() {
        deleteAll();
    }

    void GC::addRoot(Any* object) {
        _rootObjects.push_back(object);
    }

    void GC::addObject(Any* object) {
        object->setNext(_newObjects);
        _newObjects = object;
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
        while (_newObjects) {
            Any* next = _newObjects->getNext();
            delete _newObjects;
            _newObjects = next;
        }
        _allObjects = nullptr;
        _permanentObjects.clear();
        _rootObjects.clear();
    }

    void GC::dispose(std::queue<Any*>& deadObjects) {
        int nObjs = deadObjects.size();
        for (int n=0; n<nObjs; n++) {
            Any* object = deadObjects.front();
            deadObjects.pop();
            object->dispose();
        }
    }

    void GC::dump() {
        Any* object = _newObjects;
        int n = 0;
        std::cout << "GC dump:\n";
        bool committed = false;
        while (object) {
            if (object == _allObjects) {
                committed = true;
            }
            std::cout << n++ << ". ";
            std::cout << "R:" << (isRoot(object) ? "[+] " : "[_] ");
            std::cout << "C:" << (committed ? "[+] " : "[_] ");
            std::cout << "M:" << (object->isMarked() ? "[+] " : "[_] ");
            std::cout << object << " @" << (void*)object;
            std::cout << " -> " << (void*)object->getNext();
            if (_newObjects == _allObjects) {
                std::cout << ", committed";
            }
            std::cout << "\n";
            object = object->getNext();
        }
    }

    bool GC::isGCNeeded() {
        return _objectCount >= objectCountTrigger || _objectResidency >= objectResidencyTrigger;
    }

    bool GC::isCommitted(Any* object) {
        bool isCommitted = false;
        Any* objects = _newObjects;
        while (objects) {
            if (objects == _allObjects) {
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
        Any* objects = _newObjects;
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
        std::cout << "GC::mark called\n";
        std::queue<Any*> markedObjects;
        // mark all root objects
        for (Any* object : _rootObjects) {
            std::cout << "GC::mark pushing root object " << object << "\n";
            markedObjects.push(object);
        }
        // mark all new objects
        Any* newObjects = _newObjects;
        while (newObjects != _allObjects) {
            std::cout << "GC::mark pushing new object " << newObjects << "\n";
            markedObjects.push(newObjects);
            newObjects = newObjects->getNext();
        }
        std::cout << "GC::mark queue size = " << markedObjects.size() << "\n";
        while (!markedObjects.empty()) {
            Any* object = markedObjects.front();
            std::cout << "GC::mark popped object " << object << "\n";
            markedObjects.pop();
            if (!object->isMarked()) {
                object->setMarked(true);
                object->markChildren(markedObjects);
            }
        }
        std::cout << "GC:mark finished\n";
    }

    void GC::sweep(std::queue<Any*>& deadObjects) {
        Any* prev = nullptr;
        Any* object = _allObjects;
        while (object) {
            Any* next = object->getNext();
            if (object->isMarked()) {
                object->setMarked(false);
            }
            else {
                if (prev != nullptr) {
                    prev->setNext(next);
                }
                else {
                    _allObjects = next;
                }
                deadObjects.push(object);
            }
            object = next;
        }
    }

}
