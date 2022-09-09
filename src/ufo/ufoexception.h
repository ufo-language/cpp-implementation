#pragma once

#include <string>

#include "data/any.h"

namespace ufo {

    class UFOException : public std::exception {
    public:
        UFOException(std::string message, Any* payload)
            : _message{message}, _payload{payload} {}
        
    protected:
        std::string _message;
        Any* _payload;
    };

}
