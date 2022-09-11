#pragma once

namespace ufo {

    class D_List;
    class D_Nil;

    class Globals {
    public:
        Globals();
        void clear();
        D_List* emptyList() { return _emptyList; }
        D_Nil* nil() { return _nil; }
        void setup();

    protected:
        D_List* _emptyList;
        D_Nil* _nil;

    };

    extern Globals GLOBALS;

}
