#include <cassert>

#include "ufo/globals.h"
#include "data/list.h"
#include "data/nil.h"

namespace ufo {

    Globals GLOBALS;

    Globals::Globals()
        : _emptyList{nullptr}, _nil{nullptr} {
        setup();
    }

    void Globals::clear() {
        _emptyList = nullptr;
        _nil = nullptr;
    }
    
    void Globals::setup() {
        _emptyList = new D_EmptyList();
        _nil = new D_Nil();
    }

}
