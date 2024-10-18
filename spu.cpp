#include "spu.h"
#include "colors.h"
#include <stdio.h>





void Do_SPU_Init(SPU_t *spu_ptr, const char* name, const char* file, const char* func, const int line) 
{  
    
    if (!spu_ptr)
    {
        printf(ANSI_RED "%s:%d(%s) Can't construct spu:%s=NULL" ANSI_RESET_COLOR, file, line, func, name);
        return;
    }
    
        spu_ptr->name=name;
        spu_ptr->file=file;
        spu_ptr->line=line;
        spu_ptr->func=func;

        SPU_Error(spu_ptr);
        if (spu_ptr->errors != NULL_SPU_PTR && spu_ptr->errors != NO_ERROR)
            {
                printf(ANSI_RED "%s:%d(%s) problems with construct. Errors=%d" ANSI_RESET_COLOR, file, line, func, spu_ptr->errors);
                Do_SPU_Dump(spu_ptr, file, line, func);
                Do_SPU_Destroy(spu_ptr);
                return;
            }
    
}





void Do_SPU_Destroy(SPU_t* spu_ptr)
{
    SPU_Assert(spu_ptr);

    Do_SPU_Destroy(spu_ptr);
    spu_ptr->code = NULL;
    spu_ptr->ip = 0;
    spu_ptr->registers = NULL;

}





