#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdlib.h>

enum RUNNER_ERROR
{
    NO_ERR = 0,
    DIVISION_BY_ZERO=1
};


RUNNER_ERROR Run(int code[], size_t size);


enum COMANDS
{
    PUSH=1,
    PUSHR=2,
    POP=3,
    POPR=4,
    ADD=5,
    SUB=6,
    MUL=7,
    DIV=8,
    OUT=9,
    IN=10,
    DUMP=11,
    JA=12,
    JAE=13,
    JB=14,
    JBE=15,
    JE=16,
    JNE=17,
    JMP=18,
    HLT=-1
};

#endif