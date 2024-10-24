#ifndef SPU_STRUCT_H
#define SPU_STRUCT_H

#include "my_stack.h"

struct SPU_STRUCT
{
    ELEMENT_TYPE *code;
    unsigned size_code;
    int ip;
    ELEMENT_TYPE registers[10];
    unsigned size_registers;
    STACK stk;
    int ram[1024];
    ON_DEBUG
    (
        unsigned errors;
        const char* name;
        const char* file;
        const char* func;
        int line;
    )

};

typedef struct SPU_STRUCT SPU_t;





#endif