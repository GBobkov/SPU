#ifndef SPU_CHECK_H
#define SPU_CHECK_H
#include "my_stack.h"

#ifndef DEBUG
#define DEBUG
#endif

#ifdef DEBUG
    #define ON_DEBUG(code) code
#else 
    #define ON_DEBUG(code)
#endif



enum SPU_ERROR_FLAG{
    NO_ERROR=0, 
    NEGATIVE_SIZE_CODE=1 ,
    NEGATIVE_SIZE_REGISTERS=1<<1, 
    NEGATIVE_IP=1<<2,
    NULL_SPU_PTR=1<<3,
    NULL_CODE_PTR=1<<4,
    NULL_REGISTERS_PTR=1<<5,
};



struct SPU_STRUCT
{
    ELEMENT_TYPE *code;
    unsigned size_code;
    int ip;
    ELEMENT_TYPE *registers;
    unsigned size_registers;
    STACK stk;

    ON_DEBUG
    (
        unsigned errors;
        const char* name;
        const char* file;
        const char* func;
        int line;
    )

};
typedef SPU_STRUCT SPU_t;


void Do_SPU_Dump(SPU_t* spu_ptr, const char* file, const int line, const char* func);
#define SPU_Dump(spu) Do_SPU_Dump(spu, __FILE__, __LINE__, __FUNCTION__)


#ifdef DEBUG
    unsigned SPU_Error(SPU_t* spu_ptr);
    int Do_SPU_Assert(SPU_t *spu_ptr, const char* file, const int line, const char* func);
    #define SPU_Assert(spu) Do_SPU_Assert(spu, __FILE__, __LINE__, __FUNCTION__)
#endif

#endif