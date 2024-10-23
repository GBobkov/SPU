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

enum COMANDS
{
    CMD_PUSH=1,
    CMD_POP=2,
    CMD_ADD,
    CMD_SUB,
    CMD_MUL,
    CMD_DIV,
    CMD_OUT,
    CMD_IN,
    CMD_DUMP,
    CMD_JA,
    CMD_JAE,
    CMD_JB,
    CMD_JBE,
    CMD_JE,
    CMD_JNE,
    CMD_JMP,
    CMD_HLT=-1
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
    REGISTER_NAME_AX=0,
    REGISTER_NAME_BX,
    REGISTER_NAME_CX,
    REGISTER_NAME_DX,
    REGISTER_NAME_EX
};

#endif