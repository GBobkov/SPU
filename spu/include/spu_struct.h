#ifndef SPU_STRUCT_H
#define SPU_STRUCT_H

#include "my_stack.h"


const int RAM_SIZE = 128;
const int REGISTERS_NUMBER = 10;

struct SPU_STRUCT
{
    ELEMENT_TYPE *code;
    int size_code;
    int ip;
    ELEMENT_TYPE registers[REGISTERS_NUMBER];
    int size_registers;
    STACK stk;
    STACK func_call_stk;
    int ram[RAM_SIZE];
    
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