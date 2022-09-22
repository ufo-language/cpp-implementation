#pragma once

#include "data/any.h"

#if 0
struct D_StringStream* stringStream_new(char* str);
void stringStream_free(struct D_StringStream* self);

bool stringStream_boolValue(struct D_StringStream* self);
int stringStream_count(struct D_StringStream* self);
int stringStream_get(struct D_StringStream* self);
void stringStream_getPos(struct D_StringStream* self, int* pos, int* col, int* line);
void stringStream_show(struct D_StringStream* self, FILE* fp);
void stringStream_unget(struct D_StringStream* self);
size_t stringStream_sizeOf(struct D_StringStream* self);
size_t stringStream_structSize(void);
#endif

namespace ufo {

    class D_StringStream : public Any {
    public:
        static D_StringStream* create(GC::Lifetime lifetime=GC::GC_Transient);
        static D_StringStream* create(const std::string& string, GC::Lifetime lifetime=GC::GC_Transient);

        // overridden methods
        char getNextChar();
        void getPos(int& pos, int& col, int& line);
        TypeId getTypeId() override { return T_StringStream; }
        //void markChildren(std::queue<Any*>& markedObjects) override;
        void show(std::ostream& stream) override;

    protected:
        D_StringStream(const std::string& string, GC::Lifetime lifetime=GC::GC_Transient);
    };

}
