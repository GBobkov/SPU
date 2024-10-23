#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "my_stack.h"

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



#endif