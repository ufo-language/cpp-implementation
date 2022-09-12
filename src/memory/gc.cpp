#include <queue>

#include "data/any.h"
#include "memory/gc.h"

namespace ufo {

    GC THE_GC;

    GC::GC() {}

    GC::~GC() {
        deleteAll();
    }

    void GC::addObject(Any* object, Lifetime lifetime) {
        switch (lifetime) {
            case GC_Permanent:
                object->setNext(_permanentObjects);
                _permanentObjects = object;
                _nPermanentObjects++;
                break;
            case GC_Root:
                object->setNext(_rootObjects);
                _rootObjects = object;
                _nRootObjects++;
                break;
            case GC_Transient:
                object->setNext(_spine);
                _spine = object;
                _nRegisteredObjects++;
                break;
            case GC_Unmanaged:
                // if an object is originally Unmanaged, then it is safe to call addObject()
                // again with the same objct
                return;
        }
        _objectCount++;
        _memoryResidency += object->size();
    }

    void GC::collect() {
        mark();
        std::queue<Any*> deadObjects;
        sweep(deadObjects);
        dispose(deadObjects);
    }

    void GC::_deleteList(Any* list) {
        while (list != nullptr) {
            Any* next = list->getNext();
            _objectCount--;
            _memoryResidency -= list->size();
            list->dispose();
            list = next;
        }
    }
    
    void GC::deleteAll() {
        _deleteList(_spine);
        _spine = nullptr;
        _nRegisteredObjects = 0;
        _deleteList(_rootObjects);
        _rootObjects = nullptr;
        _nRootObjects = 0;
        _committedObjects = nullptr;
    }

    void GC::deletePermanentObjects() {
        _deleteList(_permanentObjects);
        _permanentObjects = nullptr;
        _nPermanentObjects = 0;
    }

    void GC::dispose(std::queue<Any*>& deadObjects) {
        int nObjs = deadObjects.size();
        for (int n=0; n<nObjs; n++) {
            Any* object = deadObjects.front();
            deadObjects.pop();
            _objectCount--;
            _memoryResidency -= object->size();
            object->dispose();
        }
    }

    void GC::dump() {
        int n = 0;
        std::cerr << "vvvvvvvvvvvvvvvv\n";
        std::cerr << "| GC dump\n";
        std::cerr << "| Spine:\n";
        bool committed = false;
        Any* object = _spine;
        while (object) {
            if (object == _committedObjects) {
                committed = true;
            }
            std::cerr << "| " << n++ << ". ";
            std::cerr << "Comm:" << (committed ? "[+] " : "[_] ");
            std::cerr << "Mark:" << (object->isMarked() ? "[+] " : "[_] ");
            std::cerr << object << " @" << (void*)object;
            std::cerr << " -> " << (void*)object->getNext();
            std::cerr << "\n";
            object = object->getNext();
        }
        std::cerr << "| Root objects:\n";
        object = _rootObjects;
        while (object) {
            std::cerr << "| " << n++ << ". ";
            std::cerr << "Mark:" << (object->isMarked() ? "[+] " : "[_] ");
            std::cerr << object << " @" << (void*)object;
            std::cerr << " -> " << (void*)object->getNext();
            std::cerr << "\n";
            object = object->getNext();
        }
        std::cerr << "| Permanent objects:\n";
        object = _permanentObjects;
        while (object) {
            std::cerr << "| " << n++ << ". ";
            std::cerr << "Mark:" << (object->isMarked() ? "[+] " : "[_] ");
            std::cerr << object << " @" << (void*)object;
            std::cerr << " -> " << (void*)object->getNext();
            std::cerr << "\n";
            object = object->getNext();
        }
        std::cerr << "^^^^^^^^^^^^^^^^\n";
    }

    bool GC::isGCNeeded() {
        return _objectCount >= objectCountTrigger || _memoryResidency >= memoryResidencyTrigger;
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

    bool GC::isPermanent(Any* object) {
        Any* object1 = _permanentObjects;
        while (object1 != nullptr) {
            if (object1 == object) {
                return true;
            }
            object1 = object1->getNext();
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
        Any* object1 = _rootObjects;
        while (object1 != nullptr) {
            if (object1 == object) {
                return true;
            }
            object1 = object1->getNext();
        }
        return false;
    }

    void GC::mark() {
        std::queue<Any*> markedObjects;
        // mark all root objects
        Any* object = _rootObjects;
        while (object != nullptr) {
            markedObjects.push(object);
            object = object->getNext();
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
        while (object != nullptr) {
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
        object = _rootObjects;
        while (object != nullptr) {
            object->setMarked(false);
            object = object->getNext();
        }
    }

}
