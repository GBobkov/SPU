#include "spu_check.h"



#include "colors.h"
#include <stdio.h>


void Do_SPU_Dump(SPU_t* spu_ptr, const char* file, const int line, const char* func)
{
    if (!spu_ptr)
    {
        printf("Assertion fault:NULL_SPU_ptr_PTR in %s:%d(%s)", file, line, func);
        return;   
    }

    printf(OPEN_LINE);
    printf(ANSI_PURPLE "spu_ptr [%p]\n", spu_ptr);
    printf("\tname=\"%s\" initializated at %s:%d(%s)\n", spu_ptr->name, spu_ptr->file, spu_ptr->line, spu_ptr->func);
    printf("\tcalled from %s:%d(%s)" ANSI_RESET_COLOR, file, line, func);
    SPU_Printf_Errors(spu_ptr->errors);
    printf("ip=%d\nsize_code=%u\nsize_registers=%u\n", spu_ptr->ip, spu_ptr->size_code, spu_ptr->size_registers);

    printf(ANSI_YELLOW "code[%p]:" ANSI_RESET_COLOR, spu_ptr->code);
    if (!spu_ptr->code)
    {
        printf(ANSI_RED "Can't print code containing: NULL_CODE_PTR" ANSI_RESET_COLOR);
        printf(END_LINE);
    }
    else
    {
        for (int i = 0; i < (int)spu_ptr->size_code ; i++)
        {
            printf(ANSI_GREEN "*[%d]=%d" ANSI_RESET_COLOR, i, spu_ptr->code[i]);
            
        }
        
        if (!spu_ptr->code) printf(ANSI_RED "Code is empty." ANSI_RESET_COLOR);
    }

    printf(ANSI_YELLOW "registers[%p]:" ANSI_RESET_COLOR, spu_ptr->registers);
    
        for (int i = 0; i < (int)spu_ptr->size_code ; i++)
            printf(ANSI_GREEN "*[%d]=%d" ANSI_RESET_COLOR, i, spu_ptr->code[i]);      
        
        if (!spu_ptr->code) printf(ANSI_RED "Code is empty." ANSI_RESET_COLOR);

    printf(END_LINE);

}


void SPU_Printf_Errors(int problem)
{

    if (problem == 0)
    {
        printf(ANSI_GREEN "NO_ERRORS" ANSI_RESET_COLOR);
        return;
    }
    printf(ANSI_RED);
    printf("ERRORS:");

    if (problem & NEGATIVE_SIZE_CODE) printf("\n\tNEGATIVE_SIZE_CODE");
    if (problem & NEGATIVE_SIZE_REGISTERS) printf("\n\tNEGATIVE_SIZE_REGISTERS");
    if (problem & NEGATIVE_IP) printf("\n\tNEGATIVE_IP");
    if (problem & NULL_SPU_PTR) printf("\n\tNULL_SPU_PTR");
    if (problem & NULL_CODE_PTR) printf("\n\tNULL_CODE_PTR");
    
    printf(ANSI_RESET_COLOR);

}




#ifdef DEBUG

unsigned SPU_Error(SPU_t* spu_ptr)
{
    unsigned errors = 0;
    if (!spu_ptr) return NULL_SPU_PTR;
    if (spu_ptr->ip <= 0) errors |= NEGATIVE_IP;
    if (spu_ptr->size_code <= 0) errors |= NEGATIVE_SIZE_CODE;
    if (spu_ptr->size_registers <= 0) errors |= NEGATIVE_SIZE_REGISTERS;
    if (!spu_ptr->code) errors |= NULL_CODE_PTR;
    
    spu_ptr->errors = errors;
    return errors;
}

int Do_SPU_Assert(SPU_t *spu_ptr, const char* file, const int line, const char* func)
{
    if (!spu_ptr)
    {
        printf("NULL_spu_ptr_PTR in %s:%d(%s)", file, line, func);
        return NULL_SPU_PTR; 
    }
    if (SPU_Error(spu_ptr))
    {   
        SPU_Dump(spu_ptr);
        return spu_ptr->errors;
    }
    return NO_ERROR;
}

#endif