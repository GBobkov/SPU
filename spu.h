#ifndef SPU_H
#define SPU_H

#include "my_stack.h"
#include "spu_check.h"


void Do_SPU_Destroy(SPU_t* spu_ptr);
#define SPU_Destroy(spu) Do_SPU_Destroy(&spu)


void Do_SPU_Init(SPU_t *spu_ptr, const char* name, const char* file, const char* func, const int line);
#define SPU_Init(spu) Do_SPU_Init(&spu, __FILE__, __LINE__, __FUNCTION__)
#endif