#pragma once

namespace ufo {

class Any;
class D_List;
class D_String;
class D_Symbol;
class Evaluator;

namespace lexer {

    // TODO move to lexer.cpp?
    enum LexerTokenType {
        LTT_None,
        LTT_Boolean,
        LTT_Nil,
        LTT_Identifier,
        LTT_Integer,
        LTT_Operator,
        LTT_Real,
        LTT_Reserved,
        LTT_Special,
        LTT_String,
        LTT_Symbol,
        LTT_Word,
        LTT_EOI,
        LTT_FINAL
    };

    // TODO move to lexer.cpp?
    extern D_Symbol* LEXER_SYMBOLS[];

    // Returns a D_Queue on success, a D_Array on error
    Any* lexer_tokenize(D_String* inputString);

}
}
