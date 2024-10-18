#ifndef MY_STACK_H
#define MY_STACK_H

#include "constants.h"


void Do_Stack_Dump(STACK* stack, const char* file, const char* func, const int line);
unsigned Do_Stack_Push(STACK* stack, ELEMENT_TYPE new_element);
ELEMENT_TYPE Do_Stack_Pop(STACK* stack);
void Do_Stack_Init(STACK *stack, size_t capacity, const char* name, const char* file, const char* func, const int line);
void Do_Stack_Destroy(STACK* stack, const char* name, const char* file, const char* func, const int line);


#endif