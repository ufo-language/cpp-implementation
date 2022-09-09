#pragma once

#include <string>

namespace ufo {

    enum TypeId {
        // T_NULL is not a type in UFO, but this entry is used for a few things.
        T_NULL = 0,
        T_Abstraction,
        T_Address,
        T_Apply,
        T_Array,
        T_Async,
        T_BinOp,
        T_Binding,
        T_Boolean,
        T_BracketExpr,
        T_Closure,
        T_Cobegin,
        T_Continuation,
        T_Do,
        T_Evaluator,
        T_ExceptionHandler,
        T_File,
        T_HashTable,
        T_Identifier,
        T_If,
        T_Integer,
        T_Iterator,
        T_Let,
        T_LetIn,
        T_LetRec,
        T_Loop,
        T_List,
        T_Nil,
        T_Nondet,
        T_Primitive,
        T_Protocol,
        T_Queue,
        T_Quote,
        T_REPL,
        T_Real,
        T_Record,
        T_RecordDefinition,
        T_RecordSpec,
        T_SavedEnv,
        T_Sequence,
        T_Set,
        T_Stream,
        T_String,
        T_StringBuffer,
        T_StringStream,
        T_Symbol,
        T_Term,
        T_Triple,
        T_TryCatch,
        T_Tuple,
        T_Unsigned,
        // T_FINAL is not a type in UFO, but this entry is used to do some
        // sanity checking on the whole table of types.
        T_FINAL
    };

    extern std::string TYPE_NAMES[T_FINAL + 1];

}
