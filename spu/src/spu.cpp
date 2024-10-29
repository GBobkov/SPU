#include "spu.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>

int Read_SPU_Data_From_File(SPU_t* spu_ptr, const char* file_name);

void Do_SPU_Init(SPU_t *spu_ptr, const char *machine_code_filename,  const char* name, const char* file, const char* func, const int line) 
{  
    
    if (!spu_ptr)
    {
        printf(ANSI_RED "%s:%d(%s) Can't construct spu:%s=NULL" ANSI_RESET_COLOR, file, line, func, name);
        return;
    }

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
                abort();
            }
    )

    spu_ptr->code = (ELEMENT_TYPE *) calloc(1024, sizeof(ELEMENT_TYPE));
    spu_ptr->ip = 0;
    
    spu_ptr->size_code = 0;
    spu_ptr->size_registers = 10;
    
    for (int i = 0; i < spu_ptr->size_registers; i++)
        spu_ptr->registers[i] = 0;
    // FIXME: demagicify!
    Stack_Init(spu_ptr->stk, 16);
    
    Stack_Init(spu_ptr->func_call_stk, 16);
    
    Read_SPU_Data_From_File(spu_ptr, machine_code_filename);

}


void Do_SPU_Destroy(SPU_t* spu_ptr)
{
    SPU_Assert(spu_ptr);
    free(spu_ptr->code);
    spu_ptr->ip = 0;
}


int Read_SPU_Data_From_File(SPU_t* spu_ptr, const char* file_name)
{
    SPU_Assert(spu_ptr);
    FILE* input_file_ptr = fopen(file_name, "r");
    ELEMENT_TYPE value = 0;
    while (fscanf(input_file_ptr, "%d", &value) > 0)
        spu_ptr->code[spu_ptr->size_code++] = value;

    fclose(input_file_ptr);
    return spu_ptr->size_code;
}