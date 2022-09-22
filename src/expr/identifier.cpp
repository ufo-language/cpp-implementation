#include "expr/identifier.h"
#include "etor/evaluator.h"

namespace ufo {

    class UnboundIdentifierException : std::exception {
    public:
        UnboundIdentifierException(E_Identifier* identifier) {
            this->identifier = identifier;
        }
        E_Identifier* identifier;
    };
    
    E_Identifier* E_Identifier::create(const std::string& name) {
        return new E_Identifier(name);
    }

    void E_Identifier::eval(Evaluator* etor) {
        Any* value = etor->lookup(this);
        if (value != nullptr) {
            etor ->pushObj(value);
        }
        throw UnboundIdentifierException(this);
    }

    void E_Identifier::show(std::ostream& stream) {
        stream << _name;
    }

}
