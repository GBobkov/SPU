#ifndef SPU_CHECK_H
#define SPU_CHECK_H

#include "spu_struct.h"

#ifndef DEBUG
#define DEBUG
#endif

#ifdef DEBUG
    #define ON_DEBUG(code) code
#else 
    #define ON_DEBUG(code)
#endif



enum SPU_ERROR_FLAG{
    NO_SPU_ERROR=0, 
    NEGATIVE_SIZE_CODE=1 ,
    NEGATIVE_SIZE_REGISTERS=1<<1, 
    NEGATIVE_IP=1<<2,
    NULL_SPU_PTR=1<<3,
    NULL_CODE_PTR=1<<4,
};








void Do_SPU_Dump(SPU_t* spu_ptr, const char* file, const int line, const char* func);
#define SPU_Dump(spu) Do_SPU_Dump(spu, __FILE__, __LINE__, __FUNCTION__)

void SPU_Printf_Errors(int problem);

#ifdef DEBUG
    unsigned SPU_Error(SPU_t* spu_ptr);
    int Do_SPU_Assert(SPU_t *spu_ptr, const char* file, const int line, const char* func);
    #define SPU_Assert(spu_ptr) Do_SPU_Assert(spu_ptr, __FILE__, __LINE__, __FUNCTION__)
#endif

#endif