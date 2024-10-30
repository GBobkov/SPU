#include "assembler.h"

#include "command_argument_handler.h"
#include "scan_labels.h"
#include "enum.h"
#include "colors.h"
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// #define BIN_OUTPUT_MODE_0
// #ifdef BIN_OUTPUT_MODE_0
// #define FPRINTF_NUMBER_IN_OUTPUT(number) fprintf(output_file_ptr, "%d\n", number);
// #elif BIN_OUTPUT_MODE_1

// // функция переводит число в двоичный вид в строке 
// static char *Int_to_Bin(int number, char* buffer, int buf_size)
// {
//     buffer += (buf_size - 1);
    
//     for (int i = 7; i >= 0; i--) {
//         *buffer-- = (number & 1) + '0';

//         number >>= 1;
//     }

//     return buffer;
// }
/*#define FPRINTF_NUMBER_IN_OUTPUT(number) const int BUF_SIZE = 8;\
        {\
            char buffer[BUF_SIZE];\
            buffer[BUF_SIZE - 1] = '\0';\
            Int_to_Bin(number, buffer, BUF_SIZE - 1);\
            fprintf(output_file_ptr, "%s\n", buffer);
        }
#endif*/


static const int COMMAND_GETS_ARGS = 1;


// функция проверяет нет ли ситуации POP num
static int Check_Pop_Args(int info_bits)
{
    if (info_bits == NUMBER_BIT)
        return 1;
    if (info_bits == (NUMBER_BIT | REGISTER_BIT))
        return 1;
    return 0;
}



static int Write_to_File(FILE *output, STACK* stk)
{
    fwrite(stk->data, sizeof(stk->data[0]), stk->size, output);
    return 0;
}



// Возвращает код ошибки. Функция обрабатывает файл с кодом.
static int Read_File_and_Fill_Machine_Code(const char *input_file_name, const char *output_file_name, LABEL* lbls)
{
    FILE* input_file_ptr = fopen(input_file_name, "r");
    FILE* output_file_ptr = fopen(output_file_name, "wb");
    STACK commands_stack = {};
    Stack_Init(commands_stack, 16);

    assert(input_file_ptr);
    assert(output_file_ptr);



    int current_file_line = 0;
    char cmd[50] = {};
    while((fscanf(input_file_ptr, "%s", cmd)  == 1))
    {
        current_file_line++;
        int cmd_code_value = 0;
    
        if (Is_Label_Name(cmd)) continue; // если имя метки, то идём на след-ую строку.

        #define CODEGEN(name) if (strcmp(cmd, #name) == 0) cmd_code_value = CMD_##name; \
                            else

        // инициализация комманды (cmd_code_value)
        CODEGEN(ADD)
        CODEGEN(SUB)
        CODEGEN(MUL)
        CODEGEN(DIV)
        CODEGEN(OUT)
        CODEGEN(IN)
        CODEGEN(DUMP)
        CODEGEN(PUSH)
        CODEGEN(POP)
        CODEGEN(JA)
        CODEGEN(JAE)
        CODEGEN(JB)
        CODEGEN(JBE)
        CODEGEN(JE)
        CODEGEN(JNE)
        CODEGEN(JMP)
        CODEGEN(RET)
        CODEGEN(CALL)
        CODEGEN(HLT)
        /*else*/
        {
            printf(ANSI_RED "UNSUPORTABLE COMAND!" ANSI_RESET_COLOR);
            abort();
        }
        #undef CODEGEN

        
        
        //FPRINTF_NUMBER_IN_OUTPUT(cmd_code_value)
        Stack_Push(commands_stack, cmd_code_value);
        
        if (cmd_code_value & COMMAND_GETS_ARGS)  // если комманда принимает аргументы, то нужно записать в машинный код её параметры
        {

            int info_bits = Write_Instruction_Arg(input_file_ptr, &commands_stack, input_file_name, current_file_line, lbls);
            //1-ый бит - есть ли чисто
            //2-ой бит - есть ли регистр
            //3-ий бит - есть ли обращение к ram


            if (cmd_code_value == CMD_POP)   // может быть ошибочная комманда "POP number", остальные ситуации для pop и push корректны
                if(Check_Pop_Args(info_bits))
                {
                    printf(ANSI_RED "incorrect pop arguments. current_file_line=%d" ANSI_RESET_COLOR, current_file_line);
                    abort();
                }

            //стоит также отметить, что jump-команды принимают любой набор аргументов.
        } 
    }

    if (current_file_line == 0)
    {
        printf(ANSI_RED "\"%s\" is empty." ANSI_RESET_COLOR, input_file_name);
        abort();      
    }

    Write_to_File(output_file_ptr, &commands_stack);
    
    Destroy_Labels_Arr(lbls);
    fclose(input_file_ptr);
    fclose(output_file_ptr);
    return 0;
}


// // Выводит массив меток.
// static void Print_Lables_Arr(LABEL *lbls)
// {
//     for (int i = 0; i < 20 && lbls[i].label_name[0] != '\0'; i++)
//     {
//         printf(ANSI_YELLOW "Lbl_name=%s\nlabel_line_position_first_meet=%d\nlabel_code_position=%d\n" ANSI_RESET_COLOR, lbls[i].label_name, lbls[i].label_line_position_first_meet,lbls[i].label_code_position);
//     }
// }



// непосредственно ассемблер. два прохода. первый - поиск меток. второй - перевод в машинный код.
int Assembler(const char *input_file_name, const char *output_file_name)
{
    LABEL* lbls = Create_Labels_Arr();
    Scan_File_Search_Labels(lbls, input_file_name);
    //Print_Lables_Arr(lbls);
    Read_File_and_Fill_Machine_Code(input_file_name, output_file_name, lbls);
    return 0;
}


