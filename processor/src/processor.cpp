#include "processor.h"

#include "spu.h"
#include "colors.h"
#include <stdlib.h>
#include <stdio.h>


// code struct like stack для того чтобы менять память т.к. code - int*


RUNNER_ERROR Run(int code[], size_t size)
{

    #define (comand) \
    if (comand == CMD_PUSH) code("==")


    SPU_t spu = {};
    SPU_Init(spu);
    
    bool running = true;
    // first second общее для всех
    while (running)
    {
        if (spu.ip >= spu.size_code) break;
        
        switch (code[spu.ip])
        {


            case CMD_PUSH:
            // spu.code[spu.ip + 1] вынести в локальну переменную в зависимости от бита
            // spu.ip + 1  -    общий для всех можно сделать до switch
                    Stack_Push(spu.stk, spu.code[spu.ip + 1]);
                    spu.ip += 2;
                    break;

            case CMD_POP:
                    Stack_Pop(spu.stk);
                    spu.ip += 1;
                    break;

            case CMD_ADD:
                    Stack_Push(spu.stk, Stack_Pop(spu.stk) + Stack_Pop(spu.stk));
                    spu.ip += 1;
                    break;

            case CMD_SUB:
                {
                    ELEMENT_TYPE second = Stack_Pop(spu.stk);
                    ELEMENT_TYPE first = Stack_Pop(spu.stk);
                    Stack_Push(spu.stk, first - second);
                    spu.ip += 1;
                    break;
                }
            case CMD_MUL:
                    Stack_Push(spu.stk, Stack_Pop(spu.stk) * Stack_Pop(spu.stk));
                    spu.ip += 1;
                    break;

            case CMD_DIV:
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

            case CMD_OUT:
                {
                    ELEMENT_TYPE top = Stack_Pop(spu.stk);
                    printf("%d\n", top);
                    Stack_Push(spu.stk, top);
                    spu.ip += 1;
                    break;
                }

            case CMD_IN:
                {
                    ELEMENT_TYPE new_element = 0;
                    scanf("%d", &new_element);
                    Stack_Push(spu.stk, new_element);
                    break;
                }
            case CMD_DUMP:
                    SPU_Dump(&spu);
                    break;

            case CMD_JA:

                {
                    ELEMENT_TYPE second = Stack_Pop(spu.stk);
                    ELEMENT_TYPE first = Stack_Pop(spu.stk);
                    if (first > second)
                        spu.ip = spu.code[spu.ip + 1];
                    break;
                }
            case CMD_JAE:
                {
                    ELEMENT_TYPE second = Stack_Pop(spu.stk);
                    ELEMENT_TYPE first = Stack_Pop(spu.stk);
                    if (first >= second)
                        spu.ip = spu.code[spu.ip + 1];
                    break;
                }
            case CMD_JB:
                {
                    ELEMENT_TYPE second = Stack_Pop(spu.stk);
                    ELEMENT_TYPE first = Stack_Pop(spu.stk);
                    if (first < second)
                        spu.ip = spu.code[spu.ip + 1];
                    break; 
                }
            case CMD_JBE:
                {
                    ELEMENT_TYPE second = Stack_Pop(spu.stk);
                    ELEMENT_TYPE first = Stack_Pop(spu.stk);
                    if (first <= second)
                        spu.ip = spu.code[spu.ip + 1];
                    break;
                }
            case CMD_JE:
                {
                    ELEMENT_TYPE second = Stack_Pop(spu.stk);
                    ELEMENT_TYPE first = Stack_Pop(spu.stk);
                    if (first == second)
                        spu.ip = spu.code[spu.ip + 1];
                    break;
                }
            case CMD_JNE:
                {
                    ELEMENT_TYPE second = Stack_Pop(spu.stk);
                    ELEMENT_TYPE first = Stack_Pop(spu.stk);
                    if (first != second)
                        spu.ip = spu.code[spu.ip + 1];
                    break;
                }
            case CMD_JMP:
                    spu.ip = spu.code[spu.ip + 1];
                    break;
            
            case CMD_HLT:
                    SPU_Destroy(spu);       // in main
                    return NO_RUNNER_ERROR;
        
            default:
                    printf(ANSI_RED "UNKNOWN COMMAND_CODE=%d in %s:%d(%s)" ANSI_RESET_COLOR, spu.code[spu.ip], __FILE__, __LINE__, __FUNCTION__);
                    abort();
                    break;
    
        }
    }

    return NO_RUNNER_ERROR;
    
}

