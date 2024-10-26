#include "assembler.h"

#include "enum.h"
#include "colors.h"
#include "my_stack.h"
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BIN_OUTPUT_MODE_0
#ifdef BIN_OUTPUT_MODE_0
#define FPRINTF_NUMBER_IN_OUTPUT(number) fprintf(output_file_ptr, "%d\n", number);
#elif BIN_OUTPUT_MODE_1
#define FPRINTF_NUMBER_IN_OUTPUT(number) const int BUF_SIZE = 8;\
        {\
            char buffer[BUF_SIZE];\
            buffer[BUF_SIZE - 1] = '\0';\
            Int_to_Bin(number, buffer, BUF_SIZE - 1);\
            fprintf(output_file_ptr, "%s\n", buffer);
        }
#endif

#define INIT_COMMAND(name) if (strcmp(cmd, #name) == 0) cmd_code_value = CMD_##name; \
    else


int Read_File_and_Fill_Machine_Code(const char *input_file_name, const char *output_file_name);




// функция проверяет нет ли ситуации POP num
int Check_The_Correctness_of_Pop_Args(int information_bits);
int Check_The_Correctness_of_Pop_Args(int information_bits)
{
    if (information_bits == 0b001)
        return 1;
        
    return 0;
}


// функция обрабатывает строку аргументов и переводит всё в машинный код 
int Write_Instruction_Arg(FILE* input_file_ptr, FILE* output_file_ptr, const char *input_file_name, int curant_file_line);


//возведение в степень
int pow(int x, int y); 
int pow(int x, int y)
{
    if (y == 0) return 1;
    return x * pow(x, y - 1);
}


// перевести строку в число (atoi) 
int String_to_Int(STACK stk, int len_of_num);
int String_to_Int(STACK stk, int len_of_num)
{
    int num = 0;
    int ten_degree = 0;
    while (len_of_num > 0)
    {
        num += pow(10, ten_degree) * Stack_Pop(stk);
        ten_degree++;
        len_of_num--;
    }
    return num;
}


// функция переводит число в двоичный вид в строке 
char *Int_to_Bin(int a, char* buffer, int buf_size);
char *Int_to_Bin(int a, char* buffer, int buf_size)
{
    buffer += (buf_size - 1);
    
    for (int i = 7; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
    }

    return buffer;
}


int Check_The_Correctness_of_Register_Name(const char *first_sym_ptr);
int Check_The_Correctness_of_Register_Name(const char *first_sym_ptr)
{
    // Первый символ - A..Z. Второй символ = 'X'. Следующим символом после регистра должен быть ' ' или '+'.
    if ((first_sym_ptr[1] != 'X') || (first_sym_ptr[2] != ' ' &&  first_sym_ptr[2] != '+' && first_sym_ptr[2] != '\0'))
    {
        printf("incorrect register_name. args_str=[%s", first_sym_ptr);
        abort();
    }
    return 0;
}



// функция возвращает номер регистра
int Find_Code_of_Register_By_Name(const char* register_name);
int Find_Code_of_Register_By_Name(const char* register_name)
{
    // AX = 1 = 'A' - 'A'  + 1
    // BX = 2 = 'B' - 'A'  + 1
    // ...
    return register_name[0] - 'A' + 1; // + 1, т.к. нулевой регистр ошибочный
}


// Возвращает код ошибки. Функция проверяет корректность расстановки квадратных скобок.
int Check_The_Correctness_of_Brackets(unsigned str_args_information_bits,  const char *args_str);
int Check_The_Correctness_of_Brackets(unsigned str_args_information_bits,  const char *args_str)
{
    // рассмотрим 3,4 биты отвечающие за наличие квадратных скобок
    if (((str_args_information_bits & 0b1000) >> 1) ^ (str_args_information_bits & 0b0100)) // сдвиг вправо на 1 бит, чтобы биты наложились
    {
        printf(ANSI_RED "Problem with brackets in command.args=%s" ANSI_RESET_COLOR, args_str);
        abort();
    }
    return 0;
}


// Возвращает код ошибки. Функция обрабатывает файл с кодом.
int Read_File_and_Fill_Machine_Code(const char *input_file_name, const char *output_file_name)
{
    FILE* input_file_ptr = fopen(input_file_name, "r");
    FILE* output_file_ptr = fopen(output_file_name, "w");

    assert(input_file_ptr);
    assert(output_file_ptr);



    int curant_file_line = 0;
    char cmd[50] = {};

    while((fscanf(input_file_ptr, "%s", cmd)  == 1))
    {
        curant_file_line++;
        int cmd_code_value = 0;
        
        // инициализация комманды (cmd_code_value)
        INIT_COMMAND(PUSH)
        INIT_COMMAND(POP)
        INIT_COMMAND(ADD)
        INIT_COMMAND(SUB)
        INIT_COMMAND(MUL)
        INIT_COMMAND(DIV)
        INIT_COMMAND(OUT)
        INIT_COMMAND(IN)
        INIT_COMMAND(DUMP)
        INIT_COMMAND(JA)
        INIT_COMMAND(JAE)
        INIT_COMMAND(JB)
        INIT_COMMAND(JBE)
        INIT_COMMAND(JE)
        INIT_COMMAND(JNE)
        INIT_COMMAND(JMP)
        INIT_COMMAND(HLT)
        /*else*/
        {
            printf(ANSI_RED "UNSUPORTABLE COMAND!" ANSI_RESET_COLOR);
            abort();
        }

        
        FPRINTF_NUMBER_IN_OUTPUT(cmd_code_value)

        if (cmd_code_value & 1)  // если комманда принимает аргументы, то нужно записать в машинный код её параметры
        {
            int information_bits = Write_Instruction_Arg(input_file_ptr, output_file_ptr, input_file_name, curant_file_line);
            //1-ый бит - есть ли чисто
            //2-ой бит - есть ли регистр
            //3-ий бит - есть ли обращение к ram


            if (cmd_code_value & 0b10)   // может быть ошибочная комманда "POP number", остальные ситуации для pop и push корректны
                if(Check_The_Correctness_of_Pop_Args(information_bits))
                {
                    printf(ANSI_RED "incorrect pop arguments. curant_file_line=%d" ANSI_RESET_COLOR, curant_file_line);
                    abort();
                }

            //стоит также отметить, что jump-команды принимают любой набор аргументов.
        } 
    }
    

    if (curant_file_line == 0)
    {
        printf(ANSI_RED "\"%s\" is empty." ANSI_RESET_COLOR, input_file_name);
        abort();      
    }

    fclose(input_file_ptr);
    fclose(output_file_ptr);
    return 0;
}


// Функция возвращает код ошибки. Считывает число со строки
int Sscanf_Number(const char* start_sym, int *symbol_index, int *output_variable);
int Sscanf_Number(const char* start_sym, int *symbol_index, int *output_variable)
{
    STACK stk_num = {};
    Stack_Init(stk_num, 16);
    int len_of_num = 0;

    // разбираем число в обратном порядке, чтобы затем его преобразовать.
    while (start_sym[len_of_num] <= '9' && start_sym[len_of_num] >= '0')
    {
        Stack_Push(stk_num, start_sym[len_of_num] - '0');
        len_of_num++;
    }
    *output_variable = String_to_Int(stk_num, len_of_num);
    Stack_Destroy(stk_num);

    *symbol_index += len_of_num - 1;     // -1 т.к. мы должны переместить указатель на конец числа, а не на след-ий символ после числа.
    return 0;               
}






// функция проверяет строку аргументов и переводит всё в машинный код
int Write_Instruction_Arg(FILE* input_file_ptr, FILE* output_file_ptr, const char *input_file_name, int curant_file_line)
{

    assert(input_file_ptr);
    assert(output_file_ptr);

    char args_str[64] = {};
    if (fscanf(input_file_ptr, "%s", args_str) <= 0)
    {
        printf(ANSI_RED "READING ERROR at %s:%d" ANSI_RESET_COLOR,input_file_name, curant_file_line);
        abort();
    } 


    unsigned str_args_information_bits = 0;
    // младший бит отвечает за наличие числа
    // второй бит отвечает за наличие регистра
    // третий бит отвечает за наличие открывающей скобки  [
    // четвёртый бит отвечает за наличие закрывающей скобки  ]

    int num_arg = 0;
    int reg_arg = 0;
    
    int symbol_index = -1;
    while (args_str[++symbol_index] != '\0')
    {
        if (args_str[symbol_index] == '[')             
            str_args_information_bits |= 0b0100;
        else if (args_str[symbol_index] == ']')
            str_args_information_bits |= 0b1000;
        else if (args_str[symbol_index] <= 'Z' && args_str[symbol_index] >= 'A')
        {
            str_args_information_bits |= 0b0010;
            Check_The_Correctness_of_Register_Name(&args_str[symbol_index]);
            reg_arg = Find_Code_of_Register_By_Name(&args_str[symbol_index]);
            symbol_index++;  // т.к. след символ 'X', его нужно пропустить.
        }
        else if (args_str[symbol_index] <= '9' && args_str[symbol_index] >= '0')
        {
            str_args_information_bits |= 0b0001;
            Sscanf_Number(&args_str[symbol_index], &symbol_index, &num_arg);
            
        }
        else if (args_str[symbol_index] == ' ' || args_str[symbol_index] == '+' || args_str[symbol_index] == '\n')
        {
            continue;
        }
        else
        {
            printf("Unpredictable command: file_name=%s, curant_file_line=%d\ncmd=%s\n", input_file_name, curant_file_line, args_str);
            abort();
        }
    }


    Check_The_Correctness_of_Brackets(str_args_information_bits, args_str);

    int value_args_information_bits = 0;
    //1-ый бит - есть ли чисто
    //2-ой бит - есть ли регистр
    //3-ий бит - есть ли обращение к ram

    value_args_information_bits |= str_args_information_bits & 0b111;           
    
    // записываем в машинный код информацию об аргументах
    FPRINTF_NUMBER_IN_OUTPUT(value_args_information_bits)   
    if (value_args_information_bits & 0b01)
        FPRINTF_NUMBER_IN_OUTPUT(num_arg)
    if (value_args_information_bits & 0b10)
        FPRINTF_NUMBER_IN_OUTPUT(reg_arg)

    return value_args_information_bits;
}



int Assembler(const char *input_file_name, const char *output_file_name)
{
    Read_File_and_Fill_Machine_Code(input_file_name, output_file_name);
    return 0;
}


