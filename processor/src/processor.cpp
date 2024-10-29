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
static int Commands_Comparator(ELEMENT_TYPE first_arg, ELEMENT_TYPE second_arg, int command)
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
static int Sum_Command_Arguments(SPU_t *spu)
{
    int sum_args = 0;
    if (spu->code[spu->ip] & NUMBER_BIT) // если есть переменная = числу
        sum_args += spu->code[spu->ip + 1];  // +1 т.к. число всегда после ячейки с информацией 
    if (spu->code[spu->ip] & REGISTER_BIT) // если есть переменная = регистру
    {
        int register_index = spu->code[spu->ip + 1 + (spu->code[spu->ip] & NUMBER_BIT)];
        int sign = (register_index >= 0) ? 1: -1;                                           // "+spu->code[spu->ip] & NUMBER_BIT" нужно для того,
        sum_args += spu->registers[abs(register_index)] * sign;                             // чтобы если число было, то мы сдвигались на два,
                                                                                            // в обратном случае на один.
    }   
    return sum_args;
}



// Функция возвращает результат бинарной операцией. 0 если такая команда не определяет операцию.
static int Get_Arg_From_Bin_Operation(ELEMENT_TYPE first, ELEMENT_TYPE second, int command)
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
static int Do_SPU_Jump(SPU_t *spu)
{
    spu->ip = Sum_Command_Arguments(spu);
    return 0;
}



// возвращает код ошибки. функция перемещает ip в зависимости от кол-ва аргументов
static int Shift_Instruction_Pointer(SPU_t * spu)
{
    int delta_ip = 0;
    // spu->code[spu->ip] - значение битов информации о параметрах 
    delta_ip += 1;  // т.к. необходимо сдвинуться вправо на 1 из-за ячейки с информацией о переменной
    delta_ip += spu->code[spu->ip] & NUMBER_BIT; // сдвиг на 1 вправо если есть переменная = числу
    delta_ip += (spu->code[spu->ip] & REGISTER_BIT) >> 1; // сдвиг на 1 вправо если есть переменная = регистру

    spu->ip += delta_ip;
    return 0;
}


// Возвращает код ошибки. Функция выполняет команду PUSH
static int Do_SPU_Push(SPU_t *spu)
{
    ELEMENT_TYPE push_value = Sum_Command_Arguments(spu); // TODO: make that if part of Sum_CommandArguments
    
    if (spu->code[spu->ip] & OPEN_BRACKET_BIT)     // если есть обращение к оперативной памяти
        {
            if (push_value < 0)
            {
                printf("PUSH VALUE < 0\n");
                abort();
            }
            push_value = spu->ram[push_value];
        }
   
    Stack_Push(spu->stk, push_value);
        
    Shift_Instruction_Pointer(spu);
    return 0;
}


// Возвращает код ошибки. Функция выполняет команду POP
static int Do_SPU_Pop(SPU_t *spu)
{
    // TODO: make separate commands for arg read & write
    // Команда записи аргументов. Команда чтения аргументов.
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


// переход на код с функцией. пополняет стэк вызовов функций. возвращает код ошибки.
static int Do_SPU_Call(SPU_t *spu)
{
    spu->ip++; // пропускаем ячейку с информацией о переменных. т.к. переменная одна - номер ip, на который делается jump.
    Stack_Push(spu->func_call_stk, spu->ip + 1); // вызов функции происходит по spu->ip, вернуться нужно на след. ячейку.
    spu->ip = spu->code[spu->ip]; // прыжок на функцию

    return 0;
}


// возвращается на последний вызов из spu->func_call_stk. возвращает код ошибки.
static int Do_SPU_Ret(SPU_t *spu)
{
    int new_ip = Stack_Pop(spu->func_call_stk);
    if (new_ip == POIZON_VALUE)
    {
        printf(ANSI_RED "func_call_stk is empty. Somewhere surplus \"RET\"." ANSI_RESET_COLOR);
        abort();
    }
    spu->ip = new_ip;

    return 0;
}



// исполняет машинный код.
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

            case CMD_CALL:
                    Do_SPU_Call(&spu);
                    break;
            
            case CMD_RET:
                    Do_SPU_Ret(&spu);
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

