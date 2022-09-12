#include <iostream>
#include <queue>
#include <sstream>
#include <string>

#include <catch2/catch.hpp>

#include "data/any.h"
#include "data/integer.h"
#include "data/list.h"
#include "data/nil.h"
#include "memory/gc.h"
#include "ufo/typeid.h"

namespace ufo {

    class TestClass : public Any {
    public:
        static int nextId;
        static int disposeCalled;
        int id;
        int markChildrenCalled = 0;
        TestClass(GC::Lifetime lifetime=GC::GC_Transient) : Any{lifetime} {
            id = nextId++;
        }
        ~TestClass() {
        }
        void dispose() {
            TestClass::disposeCalled++;
            delete this;
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
    int TestClass::disposeCalled = 0;

    TEST_CASE("gc", "[gc]") {
        THE_GC.deleteAll();
        THE_GC.deletePermanentObjects();
        GLOBALS.setup();
        TestClass::disposeCalled = 0;

        // sanity check the permanent objects
        REQUIRE_NOTHROW(GLOBALS.emptyList()->getTypeId()); 
        REQUIRE(GLOBALS.emptyList()->getTypeId() == T_List);
        REQUIRE_NOTHROW(GLOBALS.nil()->getTypeId());
        REQUIRE(GLOBALS.nil()->getTypeId() == T_Nil);

        SECTION("GC::Unmanaged") {
            TestClass* obj1 = new TestClass(GC::GC_Unmanaged);
            REQUIRE(!THE_GC.isRegistered(obj1));
            REQUIRE(!THE_GC.isRoot(obj1));
            REQUIRE(!THE_GC.isPermanent(obj1));
            delete obj1;
        }

        SECTION("GC::Transient") {
            TestClass* obj1 = new TestClass(GC::GC_Transient);
            REQUIRE(THE_GC.isRegistered(obj1));
            REQUIRE(!THE_GC.isRoot(obj1));
            REQUIRE(!THE_GC.isPermanent(obj1));
        }

        SECTION("GC::Root") {
            TestClass* obj1 = new TestClass(GC::GC_Root);
            REQUIRE(!THE_GC.isRegistered(obj1));
            REQUIRE(THE_GC.isRoot(obj1));
            REQUIRE(!THE_GC.isPermanent(obj1));
        }

        SECTION("GC::Permanent") {
            TestClass* obj1 = new TestClass(GC::GC_Permanent);
            REQUIRE(!THE_GC.isRegistered(obj1));
            REQUIRE(!THE_GC.isRoot(obj1));
            REQUIRE(THE_GC.isPermanent(obj1));
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
            TestClass* testObj1;
            TestClass* testObj2;

            SECTION("mark uncommitted non-root objects") {
                testObj1 = new TestClass();
                testObj2 = new TestClass();
                REQUIRE(!testObj1->isMarked());
                REQUIRE(!testObj2->isMarked());
                THE_GC.mark();
                REQUIRE(testObj1->isMarked());
                REQUIRE(testObj2->isMarked());
            }

            SECTION("mark committed non-root objects") {
                testObj1 = new TestClass();
                testObj2 = new TestClass();
                REQUIRE(!testObj1->isMarked());
                REQUIRE(!testObj2->isMarked());
                THE_GC.commit();
                THE_GC.mark();
                REQUIRE(!testObj1->isMarked());
                REQUIRE(!testObj2->isMarked());
            }

            SECTION("mark uncommitted with one root object") {
                testObj1 = new TestClass(GC::GC_Root);
                testObj2 = new TestClass();
                REQUIRE(!testObj1->isMarked());
                REQUIRE(!testObj2->isMarked());
                THE_GC.mark();
                REQUIRE(testObj1->isMarked());
                REQUIRE(testObj2->isMarked());
            }

            SECTION("mark committed with one root object") {
                testObj1 = new TestClass(GC::GC_Root);
                testObj2 = new TestClass();
                REQUIRE(!testObj1->isMarked());
                REQUIRE(!testObj2->isMarked());
                THE_GC.commit();
                THE_GC.mark();
                REQUIRE(testObj1->isMarked());
                REQUIRE(!testObj2->isMarked());
            }
        }

        SECTION("all phases") {
            TestClass* testObj1 = new TestClass(GC::GC_Unmanaged);
            TestClass* testObj2 = new TestClass(GC::GC_Transient);
            THE_GC.commit();
            TestClass* testObj3 = new TestClass(GC::GC_Transient);
            TestClass* testObj4 = new TestClass(GC::GC_Root);
            int numPermanents = THE_GC.getNumPermanents();
            TestClass* testObj5 = new TestClass(GC::GC_Permanent);

            REQUIRE(!THE_GC.isRegistered(testObj1));

            REQUIRE(THE_GC.isRegistered(testObj2));
            REQUIRE(THE_GC.isCommitted(testObj2));
            REQUIRE(!testObj2->isMarked());

            REQUIRE(THE_GC.isRegistered(testObj3));
            REQUIRE(!THE_GC.isCommitted(testObj3));
            REQUIRE(!testObj3->isMarked());

            REQUIRE(THE_GC.isRoot(testObj4));
            REQUIRE(!testObj4->isMarked());

            REQUIRE(THE_GC.isPermanent(testObj5));
            REQUIRE(!testObj5->isMarked());

            REQUIRE(THE_GC.getNumRegistered() == 2);
            REQUIRE(THE_GC.getNumRoots() == 1);
            REQUIRE(THE_GC.getNumPermanents() == numPermanents + 1);

            SECTION("mark sweep dispose") {
                THE_GC.mark();

                REQUIRE(!testObj1->isMarked());
                REQUIRE(!testObj2->isMarked());
                REQUIRE(testObj3->isMarked());
                REQUIRE(testObj4->isMarked());
                REQUIRE(!testObj5->isMarked());

                REQUIRE(testObj1->markChildrenCalled == 0);
                REQUIRE(testObj2->markChildrenCalled == 0);
                REQUIRE(testObj3->markChildrenCalled == 1);
                REQUIRE(testObj4->markChildrenCalled == 1);
                REQUIRE(testObj5->markChildrenCalled == 0);

                std::queue<Any*> deadObjects;
                THE_GC.sweep(deadObjects);
                REQUIRE(deadObjects.size() == 1);
                REQUIRE(deadObjects.front() == testObj2);

                THE_GC.dispose(deadObjects);

                REQUIRE(TestClass::disposeCalled == 1);
            }

            SECTION("collect") {
                THE_GC.collect();
                REQUIRE(TestClass::disposeCalled == 1);
            }
            delete testObj1;
        }

        SECTION("deleteAll permanent objects" ) {
            // sanity check the permanent objects
            REQUIRE_NOTHROW(GLOBALS.emptyList()->getTypeId()); 
            REQUIRE(GLOBALS.emptyList()->getTypeId() == T_List);
            REQUIRE_NOTHROW(GLOBALS.nil()->getTypeId());
            REQUIRE(GLOBALS.nil()->getTypeId() == T_Nil);

            REQUIRE_NOTHROW(THE_GC.deleteAll());

            // sanity check the permanent objects
            REQUIRE_NOTHROW(GLOBALS.emptyList()->getTypeId()); 
            REQUIRE(GLOBALS.emptyList()->getTypeId() == T_List);
            REQUIRE_NOTHROW(GLOBALS.nil()->getTypeId());
            REQUIRE(GLOBALS.nil()->getTypeId() == T_Nil);

            REQUIRE_NOTHROW(THE_GC.deleteAll());

            // sanity check the permanent objects
            REQUIRE_NOTHROW(GLOBALS.emptyList()->getTypeId()); 
            REQUIRE(GLOBALS.emptyList()->getTypeId() == T_List);
            REQUIRE_NOTHROW(GLOBALS.nil()->getTypeId());
            REQUIRE(GLOBALS.nil()->getTypeId() == T_Nil);
        }

        SECTION("deleteAll repeated call") {
            REQUIRE_NOTHROW(THE_GC.deleteAll());
            REQUIRE_NOTHROW(THE_GC.deleteAll());
            // non-committed + non-root
            D_Integer* i100 = D_Integer::create(100);
            REQUIRE(THE_GC.isRegistered(i100));  
            REQUIRE(!THE_GC.isCommitted(i100));
            REQUIRE(!THE_GC.isRoot(i100));
            REQUIRE_NOTHROW(THE_GC.deleteAll());
            // committed + non-root
            D_Integer* i101 = D_Integer::create(101);
            THE_GC.commit();
            REQUIRE(THE_GC.isRegistered(i101));
            REQUIRE(THE_GC.isCommitted(i101));
            REQUIRE(!THE_GC.isRoot(i101));
            REQUIRE_NOTHROW(THE_GC.deleteAll());
            // committed + root
            D_Integer* i102 = D_Integer::create(102, GC::GC_Root);
            THE_GC.commit();
            REQUIRE(!THE_GC.isCommitted(i102));
            REQUIRE(THE_GC.isRoot(i102));
            REQUIRE_NOTHROW(THE_GC.deleteAll());
        }

        THE_GC.deleteAll();
        THE_GC.deletePermanentObjects();
    }
    
}
