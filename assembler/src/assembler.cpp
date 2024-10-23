#include "../../assembler/include/assembler.h"
#include "spu/include/spu_check.h"
#include "my_stack/include/colors.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char* input_name = "assembler.txt";



int Fill_Michine_Code_From_File(SPU_t *spu_ptr, const char *file_input);

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

    if  (strcmp(cmd, "PUSH") == 0)           cmd_code_value = PUSH;
    else if (strcmp(cmd, "POP") == 0)        cmd_code_value = POP;
    else if (strcmp(cmd, "ADD") == 0)        cmd_code_value = ADD;
    else if (strcmp(cmd, "SUB") == 0)        cmd_code_value = SUB;
    else if (strcmp(cmd, "DIV") == 0)        cmd_code_value = DIV;
    else if (strcmp(cmd, "MUL") == 0)        cmd_code_value = MUL;
    else if (strcmp(cmd, "OUT") == 0)        cmd_code_value = OUT;
    else if (strcmp(cmd, "IN") == 0)         cmd_code_value = IN;
    else if (strcmp(cmd, "DUMP") == 0)       cmd_code_value = DUMP;
    else if (strcmp(cmd, "JA") == 0)         cmd_code_value = JA;
    else if (strcmp(cmd, "JAE") == 0)        cmd_code_value = JAE;
    else if (strcmp(cmd, "JB") == 0)         cmd_code_value = JB;
    else if (strcmp(cmd, "JBE") == 0)        cmd_code_value = JBE;
    else if (strcmp(cmd, "JE") == 0)         cmd_code_value = JE;
    else if (strcmp(cmd, "JNE") == 0)        cmd_code_value = JNE;
    else if (strcmp(cmd, "JMP") == 0)        cmd_code_value = JMP;
    else if (strcmp(cmd, "HLT") == 0)        cmd_code_value = HLT;
    else
    {
        printf(ANSI_RED "UNSUPORTABLE COMAND!" ANSI_RESET_COLOR);
        abort();
    }

    switch (cmd_code_value)
    {
    case PUSH:
        Write_Various_Push_Situations_To_Code(spu_ptr, input_file_ptr);
        break;

    case POP:
        Write_Various_Pop_Situations_To_Code(spu_ptr, input_file_ptr);
        break;

    case ADD:
        spu_ptr->code[spu_ptr->size_code++] = ADD;
        break;
        
    case SUB:
        spu_ptr->code[spu_ptr->size_code++] = SUB;
        break;
    
    case MUL:
        spu_ptr->code[spu_ptr->size_code++] = MUL;
        break;
    
    case DIV:
        spu_ptr->code[spu_ptr->size_code++] = DIV;
        break;
        
    case OUT:
        spu_ptr->code[spu_ptr->size_code++] = OUT;
        break;
        
    case IN:
        spu_ptr->code[spu_ptr->size_code++] = IN;
        break;
    
    case JA:
        spu_ptr->code[spu_ptr->size_code++] = JA;
        if (fscanf(input_file_ptr, "%d", &spu_ptr->code[spu_ptr->size_code++]) < 1)
        {
            printf(ANSI_RED "ERROR IN READING at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
            abort();
        }
        break;

    case JAE:
        spu_ptr->code[spu_ptr->size_code++] = JAE;
        if (fscanf(input_file_ptr, "%d", &spu_ptr->code[spu_ptr->size_code++]) < 1)
        {
            printf(ANSI_RED "ERROR IN READING at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
            abort();
        }
        break;
    
    case JB:
        spu_ptr->code[spu_ptr->size_code++] = JB;
        if (fscanf(input_file_ptr, "%d", &spu_ptr->code[spu_ptr->size_code++]) < 1)
        {
            printf(ANSI_RED "ERROR IN READING at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
            abort();
        }
        break;
    
    case JBE:
        spu_ptr->code[spu_ptr->size_code++] = JBE;
        if (fscanf(input_file_ptr, "%d", &spu_ptr->code[spu_ptr->size_code++]) < 1)
        {
            printf(ANSI_RED "ERROR IN READING at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
            abort();
        }
        break;
    
    case JE:
        spu_ptr->code[spu_ptr->size_code++] = JE;
        if (fscanf(input_file_ptr, "%d", &spu_ptr->code[spu_ptr->size_code++]) < 1)
        {
            printf(ANSI_RED "ERROR IN READING at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
            abort();
        }
        break;
    
    
    case JNE:
        spu_ptr->code[spu_ptr->size_code++] = JNE;
        if (fscanf(input_file_ptr, "%d", &spu_ptr->code[spu_ptr->size_code++]) < 1)
        {
            printf(ANSI_RED "ERROR IN READING at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
            abort();
        }
        break;
    
    case JMP:
        spu_ptr->code[spu_ptr->size_code++] = JMP;
        if (fscanf(input_file_ptr, "%d", &spu_ptr->code[spu_ptr->size_code++]) < 1)
        {
            printf(ANSI_RED "ERROR IN READING at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
            abort();
        }
        break;
    
    case HLT:
        spu_ptr->code[spu_ptr->size_code++] = HLT;
        break;
    
    default:
        printf(ANSI_RED "UNSUPORTABLE COMAND!" ANSI_RESET_COLOR);
        abort();
        break;
    }
}



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
        char register_name[16] = {};
        ELEMENT_TYPE element_value = 0;
        if (sscanf(push_args, "[%d]", &element_value) == 1)
        {
            spu_ptr->code[spu_ptr->size_code++] = ONLY_ELEMENT_PUSH_TO_RAM;
            spu_ptr->code[spu_ptr->size_code++] = element_value;
        }
        else if (sscanf(push_args, "[%s]", &register_name) == 1) 
        {
            spu_ptr->code[spu_ptr->size_code++] = ONLY_REGISTER_PUSH_TO_RAM;
            spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
        }
        else if (sscanf(push_args, "[%s + %d]", &register_name, &element_value) == 2) 
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
        else if (sscanf(push_args, "%s", &register_name) == 1) 
        {
            spu_ptr->code[spu_ptr->size_code++] = ONLY_REGISTER_PUSH_TO_SPU_STACK;
            spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
        }
        else if (sscanf(push_args, "%s + %d", &register_name, &element_value) == 2) 
            {
                spu_ptr->code[spu_ptr->size_code++] = REGISTER_AND_ELEMENT_PUSH_TO_SPU_STACK;
                spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
                spu_ptr->code[spu_ptr->size_code++] = element_value;
            }
        else
        {
            printf(ANSI_RED "UNKNOWN COMAND \"%s\" at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
            abort();
        }
    }

}


void Write_Various_Pop_Situations_To_Code(SPU_t* spu_ptr, FILE* input_file_ptr)
{
    ON_DEBUG(assert(input_file_ptr); SPU_Assert(spu_ptr);)

    char pop_args[64] = {};
        if (fscanf(input_file_ptr, "%s", pop_args) <= 0)
        {
            printf(ANSI_RED "READING ERROR at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
            abort();
        } 
    
    char register_name[16] = {};
    ELEMENT_TYPE element_value = 0;
    if (sscanf(pop_args, "[%d]", &element_value) == 1)
    {
        spu_ptr->code[spu_ptr->size_code++] = POP_ELEMENT_PUSH_TO_RAM;
        spu_ptr->code[spu_ptr->size_code++] = element_value;
    }    
    else if (sscanf(pop_args, "[%s + %d]", &register_name, &element_value) == 2) 
        {
            spu_ptr->code[spu_ptr->size_code++] = POP_ELEMENT_ADD_TO_REGISTER_PUSH_TO_RAM;
            spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
            spu_ptr->code[spu_ptr->size_code++] = element_value;
        }
    else if (sscanf(pop_args, "%s", &register_name) == 1) 
    {
        spu_ptr->code[spu_ptr->size_code++] = POP_ELEMENT_PUSH_TO_REGISTERS;
        spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
    }
    else
    {
        printf(ANSI_RED "UNKNOWN COMAND \"%s\" at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
        abort();
    }

}




REGISTERS_CODES Find_Code_of_Register_By_Name(const char* register_name, const char* file_name, int line, const char* function_name)
{
    if (strcmp(register_name, "AX") == 0) return AX;
    if (strcmp(register_name, "BX") == 0) return BX;
    if (strcmp(register_name, "CX") == 0) return CX;
    if (strcmp(register_name, "DX") == 0) return DX;
    if (strcmp(register_name, "EX") == 0) return EX;

    
    printf(ANSI_RED "UNDEFINED_REGISTER_NAME=\"%s\" at %s:%d(%s)" ANSI_RESET_COLOR, register_name, file_name, line, function_name);
    abort();
}