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

typedef SPU_STRUCT SPU_t;

enum COMANDS
{
    PUSH=1,
    POP=2,
    ADD,
    SUB,
    MUL,
    DIV,
    OUT,
    IN,
    DUMP,
    JA,
    JAE,
    JB,
    JBE,
    JE,
    JNE,
    JMP,
    HLT=-1
};

enum VARIOUS_PUSH_FLAG
{
    ONLY_ELEMENT_PUSH_TO_RAM=1,
    ONLY_REGISTER_PUSH_TO_RAM=2,
    REGISTER_AND_ELEMENT_PUSH_TO_RAM=4,
    ONLY_ELEMENT_PUSH_TO_SPU_STACK=8,
    ONLY_REGISTER_PUSH_TO_SPU_STACK=16,
    REGISTER_AND_ELEMENT_PUSH_TO_SPU_STACK=32
};

enum VARIOUS_POP_FLAG
{
    POP_ELEMENT_PUSH_TO_RAM=1,
    POP_ELEMENT_PUSH_TO_REGISTERS=2,
    POP_ELEMENT_ADD_TO_REGISTER_PUSH_TO_RAM=4
};

enum REGISTERS_CODES
{
    UNDEFINED_REGISTER_CODE=-1,
    AX=0,
    BX,
    CX,
    DX,
    EX
};

#endif