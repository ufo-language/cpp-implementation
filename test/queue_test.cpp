#include <iostream>
#include <queue>

#include <catch2/catch.hpp>

#include "data/integer.h"
#include "data/nil.h"
#include "data/queue.h"

namespace ufo {

    TEST_CASE("queue create and delete", "[queue]") {
        THE_GC.deleteAll();
        D_Queue* queue1 = new D_Queue();
        REQUIRE(queue1->isEmpty());
        REQUIRE(queue1->count() == 0);
        REQUIRE(THE_GC.isRegistered(queue1));
        THE_GC.deleteAll();
    }
 
    TEST_CASE("queue mark children", "[queue][gc]") {
        THE_GC.deleteAll();
        D_Integer* i100 = new D_Integer(100);
        D_Integer* i200 = new D_Integer(200);
        THE_GC.commit();
        D_Queue* queue1 = new D_Queue();
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

    TEST_CASE("queue enq deq count", "[queue]") {
        THE_GC.deleteAll();
        D_Integer* i100 = new D_Integer(100);
        D_Integer* i200 = new D_Integer(200);
        D_Integer* i300 = new D_Integer(300);
        D_Queue* queue1 = new D_Queue();
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

}
