#ifndef SPU_STRUCT_H
#define SPU_STRUCT_H

#include "my_stack.h"


static const int REGS_SIZE = 10;
static const int STK_SIZE = 16;
static const int RAM_SIZE = 128;
static const int CODE_SIZE = 128;

struct SPU_STRUCT
{
    ELEMENT_TYPE *code;
    int size_code;
    int ip;
    ELEMENT_TYPE registers[REGS_SIZE];
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