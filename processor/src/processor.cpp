#include "processor.h"

#include "spu.h"
#include "colors.h"
#include <stdlib.h>
#include <stdio.h>





RUNNER_ERROR Run(int code[], size_t size)
{
    SPU_t spu = {};
    SPU_Init(spu);
    
    bool running = true;
    while (running)
    {
        if (spu.ip >= spu.size_code) break;
        
        switch (code[spu.ip])
        {


            case PUSH:
                    Stack_Push(spu.stk, spu.code[spu.ip + 1]);
                    spu.ip += 2;
                    break;

            case POP:
                    Stack_Pop(spu.stk);
                    spu.ip += 1;
                    break;

            case ADD:
                    Stack_Push(spu.stk, Stack_Pop(spu.stk) + Stack_Pop(spu.stk));
                    spu.ip += 1;
                    break;

            case SUB:
                {
                    ELEMENT_TYPE second = Stack_Pop(spu.stk);
                    ELEMENT_TYPE first = Stack_Pop(spu.stk);
                    Stack_Push(spu.stk, first - second);
                    spu.ip += 1;
                    break;
                }
            case MUL:
                    Stack_Push(spu.stk, Stack_Pop(spu.stk) * Stack_Pop(spu.stk));
                    spu.ip += 1;
                    break;

            case DIV:
                {
                    ELEMENT_TYPE second = Stack_Pop(spu.stk);
                    ELEMENT_TYPE first = Stack_Pop(spu.stk);
                    if (second == 0)
                    {
                        printf(ANSI_RED "Division by zero in %s:%d(%s)\n" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
                        abort(); 
                    }

                    Stack_Push(spu.stk, first / second);
                    spu.ip += 1;
                    break;
                }

            case OUT:
                {
                    ELEMENT_TYPE top = Stack_Pop(spu.stk);
                    printf("%d\n", top);
                    Stack_Push(spu.stk, top);
                    spu.ip += 1;
                    break;
                }

            case IN:
                {
                    ELEMENT_TYPE new_element = 0;
                    scanf("%d", &new_element);
                    Stack_Push(spu.stk, new_element);
                    break;
                }
            case DUMP:
                    SPU_Dump(&spu);
                    break;

            case JA:
                {
                    ELEMENT_TYPE second = Stack_Pop(spu.stk);
                    ELEMENT_TYPE first = Stack_Pop(spu.stk);
                    if (first > second)
                        spu.ip = spu.code[spu.ip + 1];
                    break;
                }
            case JAE:
                {
                    ELEMENT_TYPE second = Stack_Pop(spu.stk);
                    ELEMENT_TYPE first = Stack_Pop(spu.stk);
                    if (first >= second)
                        spu.ip = spu.code[spu.ip + 1];
                    break;
                }
            case JB:
                {
                    ELEMENT_TYPE second = Stack_Pop(spu.stk);
                    ELEMENT_TYPE first = Stack_Pop(spu.stk);
                    if (first < second)
                        spu.ip = spu.code[spu.ip + 1];
                    break; 
                }
            case JBE:
                {
                    ELEMENT_TYPE second = Stack_Pop(spu.stk);
                    ELEMENT_TYPE first = Stack_Pop(spu.stk);
                    if (first <= second)
                        spu.ip = spu.code[spu.ip + 1];
                    break;
                }
            case JE:
                {
                    ELEMENT_TYPE second = Stack_Pop(spu.stk);
                    ELEMENT_TYPE first = Stack_Pop(spu.stk);
                    if (first == second)
                        spu.ip = spu.code[spu.ip + 1];
                    break;
                }
            case JNE:
                {
                    ELEMENT_TYPE second = Stack_Pop(spu.stk);
                    ELEMENT_TYPE first = Stack_Pop(spu.stk);
                    if (first != second)
                        spu.ip = spu.code[spu.ip + 1];
                    break;
                }
            case JMP:
                    spu.ip = spu.code[spu.ip + 1];
                    break;
            
            case HLT:
                    SPU_Destroy(spu);
                    break;
        
            default:
                    printf(ANSI_RED "UNKNOWN COMMAND_CODE=%d in %s:%d(%s)" ANSI_RESET_COLOR, spu.code[spu.ip], __FILE__, __LINE__, __FUNCTION__);
                    abort();
                    break;
    
        }
    }

    return NO_RUNNER_ERROR;
    
}

