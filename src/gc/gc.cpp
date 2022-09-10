#include <cassert>
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
        std::cout << "GC::addObject registered object at " << (void*)object << "\n";
    }

    void GC::collect() {
        mark();
        std::queue<Any*> deadObjects;
        sweep(deadObjects);
        dispose(deadObjects);
        std::cout << "GC::collect objectConut = " << _objectCount << "\n";
    }

    void GC::deleteAll() {
        std::cout << "GC::deleteAll deleting " << _objectCount << " objects\n";
        assert(_objectCount == 0 ? _committedObjects == nullptr && _spine == nullptr: true);
        while (_spine) {
            Any* next = _spine->getNext();
            //delete _spine;
            std::cout << "GC::deleteAll disposing " << TYPE_NAMES[_spine->getTypeId()] << " typeId=" << (int)_spine->getTypeId() << " @ " << (void*)_spine << "\n";
            _spine->dispose();
            _spine = next;
        }
        _spine = nullptr;
        _committedObjects = nullptr;
        _rootObjects.clear();
        _objectCount = 0;
        _objectResidency = 0;
        assert(_objectCount == 0 ? _committedObjects == nullptr && _spine == nullptr: true);
        assert(_objectCount == 0 ? _committedObjects == nullptr : true);
        assert(_committedObjects == nullptr ? _objectCount == 0 : true);
    }

    void GC::dispose(std::queue<Any*>& deadObjects) {
        int nObjs = deadObjects.size();
        for (int n=0; n<nObjs; n++) {
            Any* object = deadObjects.front();
            deadObjects.pop();
            assert(_objectCount > 0);
            _objectCount--;
            _objectResidency -= object->size();
            object->dispose();
        }
        assert(_objectCount == 0 ? _committedObjects == nullptr && _spine == nullptr: true);
    }

    void GC::dump() {
        Any* object = _spine;
        int n = 0;
        std::cout << "GC dump:\n";
        bool committed = false;
        while (object) {
            if (object == _committedObjects) {
                committed = true;
            }
            std::cout << n++ << ". ";
            std::cout << "R:" << (isRoot(object) ? "[+] " : "[_] ");
            std::cout << "C:" << (committed ? "[+] " : "[_] ");
            std::cout << "M:" << (object->isMarked() ? "[+] " : "[_] ");
            std::cout << object << " @" << (void*)object;
            std::cout << " -> " << (void*)object->getNext();
            if (_spine == _committedObjects) {
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

    void GC::sweep(std::queue<Any*>& deadObjects) {
        // unmark all root ojects
        for (Any* object : _rootObjects) {
            object->setMarked(false);
        }
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
                //printf("%s freeing obj @ %p, type %s\n ", __func__, (void*)obj, any_typeName(obj));
                //object->dispose();
                deadObjects.push(object);
            }
            object = next;
        }
    }

}
