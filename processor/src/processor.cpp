#include "processor.h"


#include "stack_check.h"



#include "enum.h"
#include "spu.h"
#include "colors.h"
#include <stdlib.h>
#include <stdio.h>


// code struct like stack для того чтобы менять память т.к. code - int* и code постоянно увеличивается
// сделать в SPU_Init


// Компаратор разбирает разные варианты комманд. 0 если комманда не найдена.
int Commands_Comparator(ELEMENT_TYPE first_arg, ELEMENT_TYPE second_arg, int command);
int Commands_Comparator(ELEMENT_TYPE first_arg, ELEMENT_TYPE second_arg, int command)
{
    #define CODEGEN(CMD_FLG, sign) if (command == CMD_FLG) return (first_arg sign second_arg) ? 1: 0;
    CODEGEN(CMD_JA, >)
    CODEGEN(CMD_JAE, >=)
    CODEGEN(CMD_JB, <)
    CODEGEN(CMD_JBE, <=)
    CODEGEN(CMD_JE, ==)
    CODEGEN(CMD_JNE, !=)
    #undef CODEGEN 

    return 0;
}



// возвращает сумму всех аргументов данной команды
int Sum_Command_Arguments(SPU_t *spu);
int Sum_Command_Arguments(SPU_t *spu)
{
    int sum_args = 0;
    if (spu->code[spu->ip] & 0b001) // если есть переменная = числу
        sum_args += spu->code[spu->ip + 1];  // +1 т.к. число всегда после ячейки с информацией 
    if (spu->code[spu->ip] & 0b010) // если есть переменная = регистру
    {
        int register_index = spu->code[spu->ip + 1 + (spu->code[spu->ip] & 0b001)]; // "+spu->code[spu->ip] & 0b001" нужно для того,
        sum_args += spu->registers[register_index];                                 // чтобы если число было, то мы сдвигались на два,
                                                                                    // в обратном случае на один.
    }   
    return sum_args;
}



// Функция возвращает результат бинарной операцией. 0 если такая команда не определяет операцию.
int Get_Arg_From_Bin_Operation(ELEMENT_TYPE first, ELEMENT_TYPE second, int command);
int Get_Arg_From_Bin_Operation(ELEMENT_TYPE first, ELEMENT_TYPE second, int command)
{
    #define CODEGEN(CMD_FLG, sign) if (command == CMD_FLG) return first sign second;
    CODEGEN(CMD_ADD, +)
    CODEGEN(CMD_SUB, -)
    CODEGEN(CMD_MUL, *)

    if (command == CMD_DIV)
    {
        if (second == 0)
            {
                printf(ANSI_RED "Division by zero in %s:%d(%s)\n" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
                abort(); 
            }
        return first / second;
    }   
    return 0;
}



// Возвращает код ошибки. Рассматривает все ситуации с аргуметами
int Do_SPU_Jump(SPU_t *spu);
int Do_SPU_Jump(SPU_t *spu)
{
    spu->ip = Sum_Command_Arguments(spu);
    return 0;
}



// возвращает код ошибки. функция перемещает ip в зависимости от кол-ва аргументов
int Shift_Instruction_Pointer(SPU_t * spu);
int Shift_Instruction_Pointer(SPU_t * spu)
{
    int delta_ip = 0;
    // spu->code[spu->ip] - значение битов информации о параметрах 
    delta_ip += 1;  // т.к. необходимо сдвинуться вправо на 1 из-за ячейки с информацией о переменной
    delta_ip += spu->code[spu->ip] & 0b001; // сдвиг на 1 вправо если есть переменная = числу
    delta_ip += (spu->code[spu->ip] & 0b010) >> 1; // сдвиг на 1 вправо если есть переменная = регистру

    spu->ip += delta_ip;
    return 0;
}


// Возвращает код ошибки. Функция выполняет команду PUSH
int Do_SPU_Push(SPU_t *spu);
int Do_SPU_Push(SPU_t *spu)
{
    ELEMENT_TYPE push_value = Sum_Command_Arguments(spu);
    
    if (spu->code[spu->ip] & 0b100)     // если есть обращение к оперативной памяти
        push_value = spu->ram[push_value];
   
   Stack_Push(spu->stk, push_value);
        
    Shift_Instruction_Pointer(spu);
    return 0;
}


// Возвращает код ошибки. Функция выполняет команду POP
int Do_SPU_Pop(SPU_t *spu);
int Do_SPU_Pop(SPU_t *spu)
{
    
    if (spu->code[spu->ip] & 0b100)  // если есть обращение к оперативной памяти
    {
        int ram_index = Sum_Command_Arguments(spu);
        spu->ram[ram_index] = Stack_Pop(spu->stk);
    }
    else if (spu->code[spu->ip] == 0b010) // если есть переменная = регистру => нужно записать в регистр верхнее число из стека
            spu->registers[spu->code[spu->ip + 1]] = Stack_Pop(spu->stk);
    else if (spu->code[spu->ip] == 0) Stack_Pop(spu->stk);
    else
    {
        printf(ANSI_RED "Wrong params for POP." ANSI_RESET_COLOR);
        Do_SPU_Dump(spu, __FILE__, __LINE__, __FUNCTION__);
        abort();
    }
    
    Shift_Instruction_Pointer(spu);
    return 0;
}



int Run(const char* machine_code_filename)
{
    
    SPU_t spu = {};
    
    SPU_Init(spu, machine_code_filename);
    SPU_Assert(&spu);

    bool running = true;
    ELEMENT_TYPE second = 0;
    ELEMENT_TYPE first = 0;
    
    while (running)
    {
        
        if (spu.ip >= spu.size_code) break;  // если массив с машинным кодом закончился
        switch (spu.code[spu.ip++])
        {
            case CMD_ADD:
            case CMD_SUB:
            case CMD_MUL:
            case CMD_DIV:
                second = Stack_Pop(spu.stk);
                first = Stack_Pop(spu.stk);
                Stack_Push(spu.stk, Get_Arg_From_Bin_Operation(first, second, spu.code[spu.ip - 1])); //  -1 в "code[..]"" т.к. (spu.ip++) в switch
                //Stack_Dump(spu.stk);
                break;

            case CMD_OUT:
                    printf("%d\n", Stack_Pop(spu.stk));
                    break;

            case CMD_IN:
                    scanf("%d", &first);
                    Stack_Push(spu.stk, first);
                    break;
                    
            case CMD_DUMP:
                    SPU_Dump(&spu);
                    break;
            
            case CMD_HLT:
                    SPU_Destroy(spu);
                    running = false;
                    break;

            case CMD_PUSH:
                    Do_SPU_Push(&spu);
                    break;

            case CMD_POP:
                    Do_SPU_Pop(&spu);
                    break;

            case CMD_JMP:
                    Do_SPU_Jump(&spu);
                    break;

            case CMD_JA:
            case CMD_JAE:
            case CMD_JB:
            case CMD_JBE:
            case CMD_JE:
            case CMD_JNE:
                    second = Stack_Pop(spu.stk);
                    first = Stack_Pop(spu.stk);
                    if (Commands_Comparator(first, second, spu.code[spu.ip - 1])) //  -1 в "code[..]"" т.к. (spu.ip++) в switch
                        Do_SPU_Jump(&spu);
                    break;
                
        
            default:
                    printf(ANSI_RED "UNKNOWN COMMAND_CODE=%d in %s:%d(%s)" ANSI_RESET_COLOR, spu.code[spu.ip], __FILE__, __LINE__, __FUNCTION__);
                    abort();
                    break;
    
        }
    }

    
    return 0;
    
}

