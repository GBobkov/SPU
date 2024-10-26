#ifndef SPU_H
#define SPU_H

#include "spu_check.h"


void Do_SPU_Destroy(SPU_t* spu_ptr);
#define SPU_Destroy(spu) Do_SPU_Destroy(&spu)


void Do_SPU_Init(SPU_t *spu_ptr, const char *machine_code_filename, const char* name, const char* file, const char* func, const int line);
#define SPU_Init(spu, machine_code_filename) Do_SPU_Init(&spu, machine_code_filename, #spu,  __FILE__, __FUNCTION__, __LINE__)

#endif