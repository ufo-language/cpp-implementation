#include <iostream>
#include <queue>

#include <catch2/catch.hpp>

#include "data/integer.h"
#include "data/nil.h"
#include "data/queue.h"

namespace ufo {

    TEST_CASE("queue", "[queue]") {
        THE_GC.deleteAll();
        THE_GC.deletePermanentObjects();
        GLOBALS.setup();

        SECTION("create", "[queue]") {
            D_Queue* queue1 = D_Queue::create();
            REQUIRE(queue1->isEmpty());
            REQUIRE(queue1->count() == 0);
            REQUIRE(THE_GC.isRegistered(queue1));
        }
 
        SECTION("mark children", "[gc]") {
            D_Integer* i100 = D_Integer::create(100);
            D_Integer* i200 = D_Integer::create(200);
            THE_GC.commit();
            D_Queue* queue1 = D_Queue::create();
            queue1->enq(i100);
            queue1->enq(i200);
            REQUIRE(!i100->isMarked());
            REQUIRE(!i200->isMarked());
            REQUIRE(!queue1->isMarked());
            THE_GC.mark();
            REQUIRE(i100->isMarked());
            REQUIRE(i200->isMarked());
            REQUIRE(queue1->isMarked());
        }

        SECTION("enq deq count") {
            D_Integer* i100 = D_Integer::create(100);
            D_Integer* i200 = D_Integer::create(200);
            D_Integer* i300 = D_Integer::create(300);
            D_Queue* queue1 = D_Queue::create();
            REQUIRE(0 == queue1->count());
            queue1->enq(i100);
            REQUIRE(1 == queue1->count());
            queue1->enq(i200);
            REQUIRE(2 == queue1->count());
            queue1->enq(i300);
            REQUIRE(3 == queue1->count());
            REQUIRE(i100 == queue1->deq());
            REQUIRE(2 == queue1->count());
            REQUIRE(i200 == queue1->deq());
            REQUIRE(1 == queue1->count());
            REQUIRE(i300 == queue1->deq());
            REQUIRE(0 == queue1->count());
        }

        THE_GC.deleteAll();
        THE_GC.deletePermanentObjects();
    }

}
