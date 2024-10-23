#include "assembler.h"
#include "my_stack.h"
#include "spu_struct.h"
#include "spu_check.h"
#include "colors.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// ассемблер не должен знать структуру процессора.
const char* input_name = "assembler.txt";

// отдельный еррор нейминг __FILE__ LINEE вообще дебаг норм сделать

#warning fix your spelling
int Fill_Michine_Code_From_File(SPU_t *spu_ptr, const char *file_input);



// new name of func write_instruction_arg
void Write_Various_Push_Situations_To_Code(SPU_t* spu_ptr, FILE* input_file_ptr);
void Write_Various_Pop_Situations_To_Code(SPU_t* spu_ptr, FILE* input_file_ptr);

REGISTERS_CODES Find_Code_of_Register_By_Name(const char* register_name, const char* file_name, int line, const char* function_name);

int Fill_Michine_Code_From_File(SPU_t *spu_ptr, const char *input_file_name)
{
    FILE *input_file_ptr = fopen(input_file_name, "r");

    ON_DEBUG(assert(input_file_ptr); SPU_Assert(spu_ptr);)

    char cmd[50] = {};
    if (fscanf(input_file_ptr, "%s", cmd) <= 0) return 1;

    int cmd_code_value = -1;


    // codegen
    if  (strcmp(cmd, "PUSH") == 0)           cmd_code_value = CMD_PUSH;   // чтение
    else if (strcmp(cmd, "POP") == 0)        cmd_code_value = CMD_POP;     // запись
    else if (strcmp(cmd, "ADD") == 0)        cmd_code_value = CMD_ADD;   
    else if (strcmp(cmd, "SUB") == 0)        cmd_code_value = CMD_SUB;      // без аргументов
    else if (strcmp(cmd, "DIV") == 0)        cmd_code_value = CMD_DIV;
    else if (strcmp(cmd, "MUL") == 0)        cmd_code_value = CMD_MUL;
    else if (strcmp(cmd, "OUT") == 0)        cmd_code_value = CMD_OUT;
    else if (strcmp(cmd, "IN") == 0)         cmd_code_value = CMD_IN;
    else if (strcmp(cmd, "DUMP") == 0)       cmd_code_value = CMD_DUMP;
    else if (strcmp(cmd, "JA") == 0)         cmd_code_value = CMD_JA;
    else if (strcmp(cmd, "JAE") == 0)        cmd_code_value = CMD_JAE;
    else if (strcmp(cmd, "JB") == 0)         cmd_code_value = CMD_JB;
    else if (strcmp(cmd, "JBE") == 0)        cmd_code_value = CMD_JBE;
    else if (strcmp(cmd, "JE") == 0)         cmd_code_value = CMD_JE;
    else if (strcmp(cmd, "JNE") == 0)        cmd_code_value = CMD_JNE;
    else if (strcmp(cmd, "JMP") == 0)        cmd_code_value = CMD_JMP;
    else if (strcmp(cmd, "HLT") == 0)        cmd_code_value = CMD_HLT;
    else
    {
        printf(ANSI_RED "UNSUPORTABLE COMAND!" ANSI_RESET_COLOR);
        abort();
    }
    // spu_ptr->code[spu_ptr->size_code++] = cmd_code_value;  выноси
    
    // убрать swicth два бита значение команды.
    switch (cmd_code_value)
    {

    case CMD_JAE:
    case CMD_JA:
    case CMD_JBE:
    case CMD_JB:
    case CMD_JE:
    case CMD_JNE:
    case CMD_JMP:
    case CMD_PUSH:
        // cmd = ...
        Write_Various_Push_Situations_To_Code(spu_ptr, input_file_ptr);

        /*if (fscanf(input_file_ptr, "%d", &spu_ptr->code[spu_ptr->size_code++]) < 1)
        {
            printf(ANSI_RED "ERROR IN READING at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
            abort();
        }*/
        break;

    case CMD_POP:
        // ...
        Write_Various_Pop_Situations_To_Code(spu_ptr, input_file_ptr);
        break;

    default:
        spu_ptr->code[spu_ptr->size_code++] = cmd_code_value;
        break;
    
        break;
    /*
    default:
        printf(ANSI_RED "UNSUPORTABLE COMAND!" ANSI_RESET_COLOR);
        abort();
        break;*/
    }

    return 0;
}


// push не делает ничего с cmd на базе write 

void Write_Various_Push_Situations_To_Code(SPU_t* spu_ptr, FILE* input_file_ptr)
{

    ON_DEBUG(assert(input_file_ptr); SPU_Assert(spu_ptr);)

    char push_args[64] = {};
        if (fscanf(input_file_ptr, "%s", push_args) <= 0)
        {
            printf(ANSI_RED "READING ERROR at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
            abort();
        } 
    if (true)
    {
        char* register_name = (char *) calloc(16, sizeof(char));

        ELEMENT_TYPE element_value = 0;
        #warning existence of [ means nothing but existence of bit flag in the result
        // отдельнеы биты для наличия числа и регистра. переменные для регистра и числа. далее if (flag && 001) code[ip++] = ......;
        
        if (sscanf(push_args, "[%d]", &element_value) == 1)
        {
            spu_ptr->code[spu_ptr->size_code++] = ONLY_ELEMENT_PUSH_TO_RAM;
            spu_ptr->code[spu_ptr->size_code++] = element_value;
        }
        else if (sscanf(push_args, "[%s]", register_name) == 1) 
        {
            spu_ptr->code[spu_ptr->size_code++] = ONLY_REGISTER_PUSH_TO_RAM;
            spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
        }
        else if (sscanf(push_args, "[%s + %d]", register_name, &element_value) == 2) 
            {
                spu_ptr->code[spu_ptr->size_code++] = REGISTER_AND_ELEMENT_PUSH_TO_RAM;
                spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
                spu_ptr->code[spu_ptr->size_code++] = element_value;
            }
        else if (sscanf(push_args, "%d", &element_value) == 1)
        {
            spu_ptr->code[spu_ptr->size_code++] = ONLY_ELEMENT_PUSH_TO_SPU_STACK;
            spu_ptr->code[spu_ptr->size_code++] = element_value;
        }
        else if (sscanf(push_args, "%s", register_name) == 1) 
        {
            spu_ptr->code[spu_ptr->size_code++] = ONLY_REGISTER_PUSH_TO_SPU_STACK;
            spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
        }
        else if (sscanf(push_args, "%s + %d", register_name, &element_value) == 2) 
            {
                spu_ptr->code[spu_ptr->size_code++] = REGISTER_AND_ELEMENT_PUSH_TO_SPU_STACK;
                spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
                spu_ptr->code[spu_ptr->size_code++] = element_value;
            }
        else
        {
            printf(ANSI_RED "UNKNOWN COMAND \"%s\" at %s:%d(%s)" ANSI_RESET_COLOR, push_args, __FILE__, __LINE__, __FUNCTION__);
            abort();
        }

        free(register_name);
    }

}


// то же самое что push, за исключение POP num. Т.о. нужно сделать общие флаги для push и pop
// вызов push, затем проверка code на корректность 
void Write_Various_Pop_Situations_To_Code(SPU_t* spu_ptr, FILE* input_file_ptr)
{
    ON_DEBUG(assert(input_file_ptr); SPU_Assert(spu_ptr);)

    char pop_args[64] = {};
        if (fscanf(input_file_ptr, "%s", pop_args) <= 0)
        {
            printf(ANSI_RED "READING ERROR at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
            abort();
        } 
    
    char* register_name = (char *) calloc(16, sizeof(char));
    
    ELEMENT_TYPE element_value = 0;
    if (sscanf(pop_args, "[%d]", &element_value) == 1)
    {
        spu_ptr->code[spu_ptr->size_code++] = POP_ELEMENT_PUSH_TO_RAM;
        spu_ptr->code[spu_ptr->size_code++] = element_value;
    }    
    else if (sscanf(pop_args, "[%s + %d]", register_name, &element_value) == 2) 
        {
            spu_ptr->code[spu_ptr->size_code++] = POP_ELEMENT_ADD_TO_REGISTER_PUSH_TO_RAM;
            spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
            spu_ptr->code[spu_ptr->size_code++] = element_value;
        }
    else if (sscanf(pop_args, "%s", register_name) == 1) 
    {
        spu_ptr->code[spu_ptr->size_code++] = POP_ELEMENT_PUSH_TO_REGISTERS;
        spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
        #warning kakogo cherta
    }
    else
    {
        printf(ANSI_RED "UNKNOWN COMAND \"%s\" at %s:%d(%s)" ANSI_RESET_COLOR, pop_args, __FILE__, __LINE__, __FUNCTION__);
        abort();
    }

    free(register_name);

}


// возможно стоит это запихать в структуру
// но это сложно инициализировать
// но можно запихать в макрос
// и это лучше чем пихать в макрос параметры

REGISTERS_CODES Find_Code_of_Register_By_Name(const char* register_name, const char* file_name, int line, const char* function_name)
{
    // вернуть разность первой буквы и 'A'
    if (strcmp(register_name, "AX") == 0) return REGISTER_NAME_AX; 
    if (strcmp(register_name, "BX") == 0) return REGISTER_NAME_BX;
    if (strcmp(register_name, "CX") == 0) return REGISTER_NAME_CX;
    if (strcmp(register_name, "DX") == 0) return REGISTER_NAME_DX;
    if (strcmp(register_name, "EX") == 0) return REGISTER_NAME_EX;

    
    printf(ANSI_RED "UNDEFINED_REGISTER_NAME=\"%s\" at %s:%d(%s)" ANSI_RESET_COLOR, register_name, file_name, line, function_name);
    abort();
}