#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stddef.h>  
#include <stdint.h>
#include "defines.h"



typedef int ELEMENT_TYPE;
typedef long long int CANARY_t;




struct STACK 
{
    ON_DEBUG
    (
        CANARY_t left_canary;
        uint64_t struct_hashsum;
        uint64_t data_hashsum;
    )

    long int size;
    long int capacity;
    ELEMENT_TYPE *data;

    ON_DEBUG
    (
        unsigned errors;
        const char* name;
        const char* file;
        const char* func;
        int line;
        CANARY_t right_canary;
    )
};


enum STACK_ERROR_FLAG{
    NO_ERROR=0, 
    NEGATIVE_SIZE=1 ,
    NEGATIVE_CAPACITY=1<<1, 
    SIZE_BIGGER_THAN_CAPACITY=1<<2,
    NULL_STACK_PTR=1<<3,
    NULL_DATA_PTR=1<<4,
    LST_CNR_DEAD=1<<5,
    RST_CNR_DEAD=1<<6,
    LDT_CNR_DEAD=1<<7,
    RDT_CNR_DEAD=1<<8,
    DAMAGE_STRUCT_HASHSUM=1<<9,
    DAMAGE_DATA_HASHSUM=1<<10,
    UNPREDICTABLE_POIZON=1<<11
};


#endif