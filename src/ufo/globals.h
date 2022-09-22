#pragma once

namespace ufo {

    class D_Array;
    class D_Boolean;
    class D_List;
    class D_Nil;
    class D_Triple;

    class Globals {
    public:
        Globals();
        void setup();
        void clear();

        D_Array* emptyArray() { return _emptyArray; }
        D_List* emptyList() { return _emptyList; }
        D_Triple* emptyTriple() { return _emptyTriple; }
        D_Nil* nil() { return _nil; }

    protected:
        D_Nil* _nil;
        D_Boolean* _true;
        D_Boolean* _false;
        D_Array* _emptyArray; 
        D_List* _emptyList;
        D_Triple* _emptyTriple;
    };

    extern Globals GLOBALS;

}
