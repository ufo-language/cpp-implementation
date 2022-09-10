#include <iostream>
#include <queue>

#include <catch2/catch.hpp>

#include "data/integer.h"
#include "data/nil.h"
#include "data/list.h"

namespace ufo {

    TEST_CASE("list create", "[list]") {
        std::cout << "list_test 'list create' starting\n";
        D_List* list1 = new D_List(NIL, NIL);
        REQUIRE(!list1->isEmpty());
        std::cout << "list_test 'list create' finished\n";
    }

    TEST_CASE("empty list", "[list]") {
        std::cout << "list_test 'empty list' starting\n";
        REQUIRE_THROWS(EMPTY_LIST->getFirst());
        REQUIRE_THROWS(EMPTY_LIST->getRest());
        REQUIRE_THROWS(EMPTY_LIST->setFirst(NIL));
        REQUIRE_THROWS(EMPTY_LIST->setRest(NIL));
        REQUIRE(EMPTY_LIST->isEmpty());
        std::cout << "list_test 'empty list' finished\n";
    }

    TEST_CASE("list get and set", "[list]") {
        std::cout << "list_test 'list get and set' starting\n";
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
        std::cout << "list_test 'list get and set' finished\n";
    }

    TEST_CASE("list mark children", "[list][gc]") {
        std::cout << "list_test 'list mark children' starting\n";
        THE_GC.deleteAll();
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
        std::cout << "list_test 'list mark children' finished\n";
    }

    TEST_CASE("list dispose", "[list]") {
        std::cout << "list_test 'list dispose' starting\n";
        std::cout << ">>>> got here 1\n";
        THE_GC.deleteAll();
        std::cout << ">>>> got here 2\n";
        D_Integer* i100 = new D_Integer(100);
        std::cout << ">>>> got here 3\n";
        D_Integer* i200 = new D_Integer(200);
        std::cout << ">>>> got here 4\n";
        D_List* list1 = new D_List(i100, i200);
        std::cout << ">>>> got here 5\n";
        (void)list1;
        std::cout << ">>>> got here 6\n";
        THE_GC.commit();
        std::cout << ">>>> got here 7\n";
        THE_GC.collect();
        std::cout << ">>>> got here 8\n";
        THE_GC.deleteAll();
        std::cout << ">>>> got here 9\n";
        std::cout << "list_test 'list dispose' finished\n";
    }

}
