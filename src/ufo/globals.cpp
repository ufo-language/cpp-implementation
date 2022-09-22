#include "data/array.h"
#include "data/boolean.h"
#include "data/list.h"
#include "data/nil.h"
#include "data/triple.h"
#include "ufo/globals.h"

namespace ufo {

    Globals GLOBALS;

    Globals::Globals()
        : _nil{nullptr},
          _true{nullptr},
          _false{nullptr},
          _emptyArray{nullptr},
          _emptyList{nullptr},
          _emptyTriple{nullptr} {
        setup();
    }

    void Globals::clear() {
        _nil = nullptr;
        _true = nullptr;
        _false = nullptr;
        _emptyArray = nullptr;
        _emptyList = nullptr;
        _emptyTriple = nullptr;
    }
    
    void Globals::setup() {
        _nil = new D_Nil();
        _true = D_Boolean::create(true);
        _false = D_Boolean::create(false);
        _emptyArray = D_Array::create(0);
        _emptyList = new D_EmptyList();
        _emptyTriple = new D_Triple(nullptr, nullptr, nullptr, GC::GC_Permanent);
    }

}
