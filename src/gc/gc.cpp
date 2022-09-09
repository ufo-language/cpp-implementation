#include <queue>

#include "data/any.h"
#include "gc/gc.h"

namespace ufo {

    GC THE_GC;

    GC::GC()
        : _allObjects{nullptr} {
    }

    GC::~GC() {
        deleteAll();
    }

    void GC::addRoot(Any* object) {
        _rootObjects.push_back(object);
    }

    void GC::addObject(Any* object) {
        object->setNext(_allObjects);
        _allObjects = object;
    }

    void GC::collect() {
        mark();
        std::queue<Any*> deadObjects;
        sweep(deadObjects);
        dispose(deadObjects);
    }

    void GC::deleteAll() {
        while (_allObjects) {
            Any* next = _allObjects->getNext();
            std::cout << "GC::deleteAll deleting " << (void*)_allObjects << " " << _allObjects << "\n";
            delete _allObjects;
            _allObjects = next;
        }
    }

    bool GC::isRegistered(Any* object) {
        Any* allObjects = _allObjects;
        while (allObjects) {
            if (object == allObjects) {
                return true;
            }
            allObjects = allObjects->getNext();
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
        std::queue<Any*> markedQ;
        for (Any* object : _rootObjects) {
            markedQ.push(object);
        }
        while (!markedQ.empty()) {
            Any* object = markedQ.front();
            markedQ.pop();
            if (!object->isMarked()) {
                object->setMarked(true);
                object->markChildren(markedQ);
            }
        }
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

    void GC::dispose(std::queue<Any*>& deadObjects) {
        int nObjs = deadObjects.size();
        for (int n=0; n<nObjs; n++) {
            Any* object = deadObjects.front();
            deadObjects.pop();
            object->dispose();
        }
    }

    void GC::dump() {
        Any* object = _allObjects;
        int n = 0;
        while (object) {
            std::cout << n++ << ". ";
            std::cout << (object->isMarked() ? "[+] " : "[_] ");
            std::cout << object << " @" << (void*)object;
            std::cout << " -> " << (void*)object->getNext();
            std::cout << "\n";
            object = object->getNext();
        }
    }

}
