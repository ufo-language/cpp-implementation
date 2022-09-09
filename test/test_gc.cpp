#include <iostream>
#include <queue>

#include <catch2/catch.hpp>

#include "data/any.h"
#include "gc/gc.h"
#include "ufo/typeid.h"

namespace ufo {

    class TestClass : public Any {
    public:
        static int nextId;
        int id;
        int disposeCalled = 0;
        int markChildrenCalled = 0;
        TestClass() : Any() {
            id = nextId++;
        }                
        void dispose() {
            this->disposeCalled++;
        }
        TypeId getTypeId() {
            return (ufo::TypeId)0;
        }
        void markChildren(std::queue<Any*>& markedObjects) {
            (void)markedObjects;
            this->markChildrenCalled++;
        }
        void show(std::ostream& stream) {
            stream << "TEST-OBJECT-" << id;
        }
    };

    int TestClass::nextId = 0;

    TEST_CASE("all phases", "[gc]") {
        TestClass* testObj1 = new TestClass();
        TestClass* testObj2 = new TestClass();

        REQUIRE(THE_GC.isRegistered(testObj1));
        REQUIRE(THE_GC.isRegistered(testObj2));
        REQUIRE(!THE_GC.isRoot(testObj1));
        REQUIRE(!THE_GC.isRoot(testObj2));
        REQUIRE(!testObj1->isMarked());
        REQUIRE(!testObj2->isMarked());

        THE_GC.addRoot(testObj1);

        REQUIRE(THE_GC.isRoot(testObj1));
        REQUIRE(!THE_GC.isRoot(testObj2));

        SECTION("mark sweep dispose") {
            THE_GC.mark();

            REQUIRE(testObj1->isMarked());
            REQUIRE(!testObj2->isMarked());

            std::queue<Any*> deadObjects;
            THE_GC.sweep(deadObjects);

            REQUIRE(deadObjects.size() == 1);
            REQUIRE(deadObjects.front() == testObj2);

            THE_GC.dispose(deadObjects);

            REQUIRE(testObj1->disposeCalled == 0);
            REQUIRE(testObj2->disposeCalled == 1);
        }

        SECTION("collect") {
            THE_GC.collect();

            REQUIRE(testObj1->disposeCalled == 0);
            REQUIRE(testObj2->disposeCalled == 1);
        }

        delete testObj2;
    }
    
}
