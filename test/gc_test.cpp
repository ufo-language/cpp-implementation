#include <iostream>
#include <queue>

#include <catch2/catch.hpp>

#include "data/any.h"
#include "data/integer.h"
#include "data/list.h"
#include "gc/gc.h"
#include "ufo/typeid.h"

namespace ufo {

    class TestClass : public Any {
    public:
        static int nextId;
        int id;
        int disposeCalled = 0;
        int markChildrenCalled = 0;
        TestClass() : Any{T_NULL} {
            id = nextId++;
        }
        ~TestClass() {
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

    TEST_CASE("gc", "[gc]") {

        THE_GC.deleteAll();

        SECTION("register") {
            D_Integer* i100 = new D_Integer(100);
            D_Integer* i200 = new D_Integer(200);
            D_List* list1 = new D_List(i100, i200);
            REQUIRE(THE_GC.isRegistered(i100));
            REQUIRE(THE_GC.isRegistered(i200));
            REQUIRE(THE_GC.isRegistered(list1));
        }

        SECTION("register and commit") {
            TestClass* testObj1 = new TestClass();
            REQUIRE(!THE_GC.isCommitted(testObj1));
            THE_GC.commit();
            REQUIRE(THE_GC.isCommitted(testObj1));
            TestClass* testObj2 = new TestClass();
            REQUIRE(THE_GC.isCommitted(testObj1));
            REQUIRE(!THE_GC.isCommitted(testObj2));
            THE_GC.commit();
            REQUIRE(THE_GC.isCommitted(testObj1));
            REQUIRE(THE_GC.isCommitted(testObj2));
        }

        SECTION("mark") {
            TestClass* testObj1 = new TestClass();
            TestClass* testObj2 = new TestClass();
            REQUIRE(!testObj1->isMarked());
            REQUIRE(!testObj2->isMarked());

            SECTION("mark uncommitted non-root objects") {
                THE_GC.mark();
                REQUIRE(testObj1->isMarked());
                REQUIRE(testObj2->isMarked());
            }

            SECTION("mark committed non-root objects") {
                THE_GC.commit();
                THE_GC.mark();
                REQUIRE(!testObj1->isMarked());
                REQUIRE(!testObj2->isMarked());
            }

            SECTION("mark uncommitted with one root object") {
                THE_GC.addRoot(testObj1);
                THE_GC.mark();
                REQUIRE(testObj1->isMarked());
                REQUIRE(testObj2->isMarked());
            }

            SECTION("mark committed with one root object") {
                THE_GC.addRoot(testObj1);
                THE_GC.commit();
                THE_GC.mark();
                REQUIRE(testObj1->isMarked());
                REQUIRE(!testObj2->isMarked());
            }
        }

        SECTION("all phases") {
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

            THE_GC.commit();

            SECTION("mark sweep dispose") {
                THE_GC.mark();

                REQUIRE(testObj1->isMarked());
                REQUIRE(!testObj2->isMarked());
                REQUIRE(testObj1->markChildrenCalled == 1);
                REQUIRE(testObj2->markChildrenCalled == 0);

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
        }

        SECTION("deleteAll repeated call") {
            REQUIRE_NOTHROW(THE_GC.deleteAll());
            REQUIRE_NOTHROW(THE_GC.deleteAll());
            // non-committed + non-root
            D_Integer* i100 = new D_Integer(100);
            REQUIRE(THE_GC.isRegistered(i100));  
            REQUIRE(!THE_GC.isCommitted(i100));
            REQUIRE(!THE_GC.isRoot(i100));
            REQUIRE_NOTHROW(THE_GC.deleteAll());
            // committed + non-root
            D_Integer* i101 = new D_Integer(101);
            THE_GC.commit();
            REQUIRE(THE_GC.isRegistered(i101));
            REQUIRE(THE_GC.isCommitted(i101));
            REQUIRE(!THE_GC.isRoot(i101));
            REQUIRE_NOTHROW(THE_GC.deleteAll());
            // committed + root
            D_Integer* i102 = new D_Integer(102);
            THE_GC.addRoot(i102);
            THE_GC.commit();
            REQUIRE(THE_GC.isRegistered(i102));
            REQUIRE(THE_GC.isCommitted(i102));
            REQUIRE(THE_GC.isRoot(i102));
            REQUIRE_NOTHROW(THE_GC.deleteAll());
        }
    }
    
}
