#include "spu.h"
#include "colors.h"
#include <stdio.h>

static const char* machine_code_filename = "machine_code.txt";


int Read_SPU_Data_From_File(SPU_t* spu_ptr, const char* file_name);

void Do_SPU_Init(SPU_t *spu_ptr, const char* name, const char* file, const char* func, const int line) 
{  
    
    if (!spu_ptr)
    {
        printf(ANSI_RED "%s:%d(%s) Can't construct spu:%s=NULL" ANSI_RESET_COLOR, file, line, func, name);
        return;
    }
    
    Read_SPU_Data_From_File(spu_ptr, machine_code_filename);

    Stack_Init(spu_ptr->stk, 16);


    ON_DEBUG
    (
        spu_ptr->name=name;
        spu_ptr->file=file;
        spu_ptr->line=line;
        spu_ptr->func=func;
        if (spu_ptr->errors != NULL_SPU_PTR && spu_ptr->errors != NO_ERROR)
            {
                printf(ANSI_RED "%s:%d(%s) problems with construct. Errors=%d" ANSI_RESET_COLOR, file, line, func, spu_ptr->errors);
                Do_SPU_Dump(spu_ptr, file, line, func);
                Do_SPU_Destroy(spu_ptr);
            }
    )
}


void Do_SPU_Destroy(SPU_t* spu_ptr)
{
    SPU_Assert(spu_ptr);
    spu_ptr->code = NULL;
    spu_ptr->ip = 0;
}


int Read_SPU_Data_From_File(SPU_t* spu_ptr, const char* file_name)
{
    FILE* input_file = fopen(file_name, "r");
    int value = 0;
    while (fscanf(input_file, "%d", &value) > 0)
        spu_ptr->code[spu_ptr->size_code++] = value;

    
    return spu_ptr->size_code;
}