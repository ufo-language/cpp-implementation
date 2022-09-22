#include <array>
#include <sstream>

#include "data/any.h"
#include "data/array.h"
#include "data/boolean.h"
#include "data/integer.h"
#include "data/nil.h"
#include "data/queue.h"
#include "data/real.h"
#include "data/string.h"
#include "data/stringstream.h"
#include "data/symbol.h"
#include "expr/identifier.h"
#include "lexer/lexer.h"
#include "ufo/globals.h"

namespace ufo {
namespace lexer {

    static bool contains(const std::string& str, char c);
    static bool isReserved(const std::string& word);
    static Any* tokenize_aux(D_StringStream* inputString);

    static std::string operChars = ".:+-*/=%";
    static std::string ignoreChars = " \n\r\t";
    static std::array<std::string, 17> reservedWords =
       {"async",
        "catch",
        "cobegin",
        "do",
        "else",
        "end",
        "fun",
        "if",
        "in",
        "let",
        "letrec",
        "loop",
        "macro",
        "nondet",
        "record",
        "then",
        "try"
       };

    D_Symbol* LEXER_SYMBOLS[LTT_FINAL];

    static std::string TRUE_WORD;
    static std::string FALSE_WORD;
    static std::string NIL_WORD;

    void lexer_rootObjects(void) {
        LEXER_SYMBOLS[LTT_None]       = D_Symbol::create("None");
        LEXER_SYMBOLS[LTT_Boolean]    = D_Symbol::create("Boolean");
        LEXER_SYMBOLS[LTT_Nil]        = D_Symbol::create("Nil");
        LEXER_SYMBOLS[LTT_Identifier] = D_Symbol::create("Identifier");
        LEXER_SYMBOLS[LTT_Integer]    = D_Symbol::create("Integer");
        LEXER_SYMBOLS[LTT_Operator]   = D_Symbol::create("Operator");
        LEXER_SYMBOLS[LTT_Real]       = D_Symbol::create("Real");
        LEXER_SYMBOLS[LTT_Reserved]   = D_Symbol::create("Reserved");
        LEXER_SYMBOLS[LTT_Special]    = D_Symbol::create("Special");
        LEXER_SYMBOLS[LTT_String]     = D_Symbol::create("String");
        LEXER_SYMBOLS[LTT_Symbol]     = D_Symbol::create("Symbol");
        LEXER_SYMBOLS[LTT_Word]       = D_Symbol::create("Word");
        LEXER_SYMBOLS[LTT_EOI]        = D_Symbol::create("EOI");
    }

    void lexer_initialize(void) {
        NIL_WORD = "nil";
        TRUE_WORD = "true";
        FALSE_WORD = "false";
    }

    D_Array* makeToken(enum LexerTokenType tokenType, std::stringstream& lexeme, int pos, int col, int line) {
        std::string lexemeString = lexeme.str();
        lexeme.clear();
        lexeme.str(std::string());
        D_Symbol* tokenTypeSym = NULL;
        Any* token = NULL;
        switch (tokenType) {
            case LTT_Integer:
                tokenTypeSym = LEXER_SYMBOLS[LTT_Integer];
                {    
                    int intValue = stoi(lexemeString);
                    token = D_Integer::create(intValue);
                }
                break;
            case LTT_Real:
                tokenTypeSym = LEXER_SYMBOLS[LTT_Real];
                {
                    double doubleValue = stof(lexemeString);
                    token = D_Real::create(doubleValue);
                }
                break;
            case LTT_Special:
                tokenTypeSym = LEXER_SYMBOLS[LTT_Special];
                token = D_String::create(lexemeString);
                break;
            case LTT_String:
                tokenTypeSym = LEXER_SYMBOLS[LTT_String];
                token = D_String::create(lexemeString);
                break;
            case LTT_Symbol:
                tokenTypeSym = LEXER_SYMBOLS[LTT_Symbol];            
                token = D_Symbol::create(lexemeString);
                break;
            case LTT_Operator:
                tokenTypeSym = LEXER_SYMBOLS[LTT_Operator];
                token = E_Identifier::create(lexemeString);
                break;
            case LTT_Word:
                if (TRUE_WORD == lexemeString) {
                    tokenTypeSym = LEXER_SYMBOLS[LTT_Boolean];
                    token = D_Boolean::create(true);
                }
                else if (FALSE_WORD == lexemeString) {
                    tokenTypeSym = LEXER_SYMBOLS[LTT_Boolean];
                    token = D_Boolean::create(false);
                }
                else if (NIL_WORD == lexemeString) {
                    tokenTypeSym = LEXER_SYMBOLS[LTT_Nil];
                    token = GLOBALS.nil();
                }
                else if (isReserved(lexemeString)) {
                    tokenTypeSym = LEXER_SYMBOLS[LTT_Reserved];
                    token = D_String::create(lexemeString);
                }
                else {
                    tokenTypeSym = LEXER_SYMBOLS[LTT_Identifier];
                    token = E_Identifier::create(lexemeString);
                }
                break;
            case LTT_EOI:
                tokenTypeSym = LEXER_SYMBOLS[LTT_EOI];
                token = GLOBALS.nil();
                break;
            case LTT_None:
            case LTT_Boolean:
            case LTT_Nil:
            case LTT_Identifier:
            case LTT_Reserved:
            case LTT_FINAL:
                printf("%s got unhandled token type %d\n", __func__, tokenType);
        }
        return D_Array::create({tokenTypeSym, token, D_Integer::create(pos), D_Integer::create(col), D_Integer::create(line)});
    }

    Any* lexer_tokenize(D_String* inputString) {
        D_StringStream* inputStream = D_StringStream::create(inputString->getString());
        Any* res = tokenize_aux(inputStream);
        if (res->isA(T_Queue)) {
            return ((D_Queue*)res)->asList();
        }
        return res;
    }

    // Returns a D_Queue on success, a D_Array on error
    static Any* tokenize_aux(D_StringStream* inputString) {
        enum StateId {
            S_Initial,        // 0
            S_Word,           // 1
            S_Number,         // 2
            S_Symbol,         // 3
            S_String,         // 4
            S_EscapedChar,    // 5
            S_Operator,       // 6
            S_Plus,           // 7
            S_Minus,          // 8
            S_Dot,            // 9
            S_Real,           // 10
            S_Comment,        // 11
            S_LineComment,    // 12
            S_BlockComment1,  // 13
            S_BlockComment2   // 13
        };
        enum StateId state = S_Initial;
        D_Queue* tokens = D_Queue::create();
        std::stringstream lexeme;
        int pos, col, line;
        bool continueLoop = true;
        int chr;
        int ungetChar = -1;
        while (continueLoop) {
            // get the next character, or the last un-got character
            if (ungetChar > -1) {
                chr = ungetChar;
                ungetChar = -1;
            }
            else {
                chr = inputString->getNextChar();
            }
            bool accumulateChar = false;
            enum LexerTokenType tokenType = LTT_None;
            // handle the character
            switch (state) {
                case S_Initial:  // 0
                    inputString->getPos(pos, line, col);
                    if      (chr >= 'A' && chr <= 'Z')   { accumulateChar = true; state = S_Symbol; }
                    else if (chr >= 'a' && chr <= 'z')   { accumulateChar = true; state = S_Word; }
                    else if (chr >= '0' && chr <= '9')   { accumulateChar = true; state = S_Number; }
                    else if (chr == '+')                 { state = S_Plus; }
                    else if (chr == '-')                 { state = S_Minus; }
                    else if (chr == '"')                 { state = S_String; }
                    else if (contains(operChars, chr))   { accumulateChar = true; state = S_Operator; }
                    else if (contains(ignoreChars, chr)) { }
                    else if (chr == ';')                 { state = S_Comment; }
                    else if (chr == '\0')                { continueLoop = false; }
                    else {
                        tokenType = LTT_Special;
                        accumulateChar = true;
                    }
                    break;

                case S_Word:  // 1
                    if      (chr >= 'A' && chr <= 'Z') { accumulateChar = true; }
                    else if (chr >= 'a' && chr <= 'z') { accumulateChar = true; }
                    else if (chr >= '0' && chr <= '9') { accumulateChar = true; }
                    else if (chr == '_')               { accumulateChar = true; }
                    else {
                        tokenType = LTT_Word;
                        ungetChar = chr;
                    }
                    break;

                case S_Number:  // 2
                    if      (chr >= '0' && chr <= '9') { accumulateChar = true; }
                    else if (chr == '.') { accumulateChar = true; state = S_Dot; }
                    else {
                        tokenType = LTT_Integer;
                        ungetChar = chr;
                    }
                    break;

                case S_Symbol:  // 3
                    if      (chr >= 'A' && chr <= 'Z') { accumulateChar = true; }
                    else if (chr >= 'a' && chr <= 'z') { accumulateChar = true; }
                    else if (chr >= '0' && chr <= '9') { accumulateChar = true; }
                    else if (chr == '_')               { accumulateChar = true; }
                    else {
                        tokenType = LTT_Symbol;
                        ungetChar = chr;
                    }
                    break;

                case S_String:  // 4
                    if      (chr == '\\') { state = S_EscapedChar; }
                    else if (chr == '"')  { tokenType = LTT_String; }
                    else if (chr == '\0') {
                        D_List* posBinding = D_List::create(D_Symbol::create("Pos"), D_Integer::create(pos));
                        D_List* lineBinding = D_List::create(D_Symbol::create("Line"), D_Integer::create(line));
                        D_List* colBinding = D_List::create(D_Symbol::create("Col"), D_Integer::create(col));
                        D_Array* exn = D_Array::create(
                            {D_Symbol::create("Lexer"),
                             D_String::create("unterminated string"),
                             D_Array::create({posBinding, lineBinding, colBinding})
                            });
                        return exn;
                    }
                    else { accumulateChar = true; }
                    break;

                case S_EscapedChar:  // 5
                    switch (chr) {
                        case 'n': chr = '\n'; break;
                        case 'r': chr = '\r'; break;
                        case 't': chr = '\t'; break;
                        case '0': chr = '\0'; break;
                        default:
                            break;
                    }
                    accumulateChar = true;
                    state = S_String;
                    break;

                case S_Operator:  // 6
                    if (contains(operChars, chr)) { accumulateChar = true; }
                    else {
                        tokenType = LTT_Operator;
                        ungetChar = chr;
                    }
                    break;

                case S_Plus:  // 7
                    if (chr >= '0' && chr <= '9') { accumulateChar = true; state = S_Number; }
                    else {
                        lexeme << '+';
                        ungetChar = chr;
                        state = S_Operator;
                    }
                    break;

                case S_Minus:  // 8
                    if (chr >= '0' && chr <= '9') {
                        lexeme << '-';
                        accumulateChar = true;
                        state = S_Number;
                    }
                    else {
                        lexeme << '-';
                        ungetChar = chr;
                        state = S_Operator;
                    }
                    break;

                case S_Dot:  // 9
                    if (chr >= '0' && chr <= '9')  { accumulateChar = true; state = S_Real; }
                    else {
                        tokenType = LTT_Real;
                        lexeme << '0';
                        ungetChar = chr;
                    }
                    break;

                case S_Real:  // 10
                    if (chr >= '0' && chr <= '9')  { accumulateChar = true; }
                    else {
                        tokenType = LTT_Real;
                        ungetChar = chr;
                    }
                    break;

                case S_Comment:  // 11
                    if (chr == '-') {
                        state = S_BlockComment1;
                    }
                    else if (chr == '\n' || chr == '\0') {
                        state = S_Initial;
                    }
                    else {
                        state = S_LineComment;
                    }
                    break;

                case S_LineComment:  // 12
                    if (chr == '\n' || chr == '\0') {
                        state = S_Initial;
                    }
                    break;

                case S_BlockComment1:  // 13
                    if (chr == '-') {
                        state = S_BlockComment2;
                    }
                    else if (chr == '\0') {
                        D_List* posBinding = D_List::create(D_Symbol::create("Pos"), D_Integer::create(pos));
                        D_List* lineBinding = D_List::create(D_Symbol::create("Line"), D_Integer::create(line));
                        D_List* colBinding = D_List::create(D_Symbol::create("Col"), D_Integer::create(col));
                        D_Array* exn = D_Array::create(
                            {D_Symbol::create("Lexer"),
                             D_String::create("unterminated block comment"),
                             D_Array::create(
                                 {posBinding, lineBinding, colBinding})
                            });
                        return exn;
                    }
                    break;

                case S_BlockComment2:  // 14
                    if (chr == ';') {
                        state = S_Initial;
                    }
                    else {
                        state = S_BlockComment1;
                    }
                    break;

                default:
                    printf("%s: illegal state %d\n", __func__, state);
                    continueLoop = false;
            }
            if (accumulateChar) {
                lexeme << chr;
            }
            if (tokenType != LTT_None) {
                D_Array* token = makeToken(tokenType, lexeme, pos, col, line);
                tokens->enq(token);
                state = S_Initial;
            }
        }
        if (tokens->count() > 0) {
            D_Array* token = makeToken(LTT_EOI, lexeme, pos, col, line);
            tokens->enq(token);
        }
        return tokens;
    }

    // support functions =================================================

    // determines if a character is in a string
    static bool contains(const std::string& str, char c) {
        return str.find(c) != std::string::npos;
    }

    static bool isReserved(const std::string& word) {
        for (const std::string& reservedWord : reservedWords) {
            if (reservedWord == word) {
                return true;
            }
        }
        return false;
    }

}
}
