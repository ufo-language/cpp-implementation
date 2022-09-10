#include "data/any.h"
#include "gc/gc.h"
#include "ufo/typeid.h"

namespace ufo {

    extern GC THE_GC;

    Any::Any(TypeId typeId)
        : _typeId{typeId} {
        THE_GC.addObject(this);
    }

    Any::~Any() {
    }

    std::ostream& operator << (std::ostream& stream, std::nullptr_t) {
        return stream << "nullptr";
    }

    std::ostream& operator<<(std::ostream &stream, Any* object) {
        if(object == nullptr) {
            stream << "NULL-OBJECT";
        }
        else {
            object->show(stream);
        }
        return stream;
    }

}
