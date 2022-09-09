#include <iostream>
#include <queue>

#include <catch2/catch.hpp>

#include "data/integer.h"
#include "data/list.h"

namespace ufo {

    TEST_CASE("create", "[list]") {
        D_Integer* i100 = new D_Integer(100);
        D_Integer* i200 = new D_Integer(200);
        D_List* list1 = new D_List(i100, i200);
        REQUIRE(THE_GC.isRegistered(i100));
        REQUIRE(THE_GC.isRegistered(i200));
        REQUIRE(THE_GC.isRegistered(list1));
    }
    
}
