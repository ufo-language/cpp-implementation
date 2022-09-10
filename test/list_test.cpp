#include <iostream>
#include <queue>

#include <catch2/catch.hpp>

#include "data/integer.h"
#include "data/nil.h"
#include "data/list.h"

namespace ufo {

    TEST_CASE("list", "[list]") {

        THE_GC.deleteAll();

        SECTION("create") {
            REQUIRE(EMPTY_LIST->isEmpty());
            D_List* list1 = new D_List(NIL, NIL);
            REQUIRE(!list1->isEmpty());
        }

        SECTION("mark children", "[gc]") {
            D_Integer* i100 = new D_Integer(100);
            D_Integer* i200 = new D_Integer(200);
            THE_GC.commit();
            D_List* list1 = new D_List(i100, i200);
            REQUIRE(!i100->isMarked());
            REQUIRE(!i200->isMarked());
            REQUIRE(!list1->isMarked());
            THE_GC.mark();
            REQUIRE(i100->isMarked());
            REQUIRE(i200->isMarked());
            REQUIRE(list1->isMarked());        
        }

        SECTION("empty list") {
            REQUIRE_THROWS(EMPTY_LIST->getFirst());
            REQUIRE_THROWS(EMPTY_LIST->getRest());
            REQUIRE_THROWS(EMPTY_LIST->setFirst(NIL));
            REQUIRE_THROWS(EMPTY_LIST->setRest(NIL));
            REQUIRE(EMPTY_LIST->isEmpty());
        }

        SECTION("get and set") {
            D_Integer* i100 = new D_Integer(100);
            D_Integer* i200 = new D_Integer(200);
            D_List* list1 = new D_List(i100, i200);
            REQUIRE(i100 == list1->getFirst());
            REQUIRE(i200 == list1->getRest());
            D_Integer* i300 = new D_Integer(300);
            D_Integer* i400 = new D_Integer(400);
            list1->setFirst(i300);
            list1->setRest(i400);
            REQUIRE(i300 == list1->getFirst());
            REQUIRE(i400 == list1->getRest());
        }
    }

}
