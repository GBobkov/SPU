#include "assembler.h"

#include "enum.h"
#include "colors.h"
#include "my_stack.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INIT_COMMAND(name) if (strcmp(cmd, #name) == 0) cmd_code_value = CMD_##name; \
    else


int Read_File_and_Fill_Machine_Code(const char *input_file_name, const char *output_file_name);





// функция проверяет нет ли ситуации POP num
int Check_The_Correctness_of_Pop_Args(int information_bits);
int Check_The_Correctness_of_Pop_Args(int information_bits)
{
    return information_bits; // in development
}


// функция обрабатывает строку аргументов и переводит всё в машинный код 
int Write_Instruction_Arg(FILE* input_file_ptr, FILE* output_file_ptr, const char *input_file_name, int curant_file_line);

//возведение в степень
int pow(int x, int y); 

// перевести строку в число (atoi) 
int String_to_Int(STACK stk, int len_of_num);

// функция переводит число в двоичный вид в строке 
char *Int_to_Bin(int a, char* buffer, int buf_size);

char *Int_to_Bin(int a, char* buffer, int buf_size)
{
    buffer += (buf_size - 1);
    
    for (int i = 31; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
    }

    return buffer;
}


int Check_The_Correctness_of_Register_Name(const char *first_sym_ptr);
int Check_The_Correctness_of_Register_Name(const char *first_sym_ptr)
{
    return first_sym_ptr[0] - '0';  // in development
}




int Find_Code_of_Register_By_Name(const char* register_name);

int Find_Code_of_Register_By_Name(const char* register_name)
{
    // AX = 1 = 'A' - 'A'  + 1
    // BX = 2 = 'B' - 'A'  + 1
    // ...
    return register_name[0] - 'A' + 1;
}


int Check_The_Correctness_of_Brackets(const char *arg_str);
int Check_The_Correctness_of_Brackets(const char *arg_str)
{
    return arg_str[0] -'0';
}

int Read_File_and_Fill_Machine_Code(const char *input_file_name, const char *output_file_name)
{

    FILE *input_file_ptr = fopen(input_file_name, "r");
    FILE *output_file_ptr = fopen(output_file_name, "w");

    assert(input_file_ptr);
    assert(output_file_ptr);

    int line = 0;
    char cmd[50] = {};

    while((fscanf(input_file_ptr, "%s", cmd)  == 1))
    {
        line++;
        int cmd_code_value = 0;
        

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
            exit(1);
        }

        
        fprintf(output_file_ptr, "%d\n", cmd_code_value);  // код команды

        if (cmd_code_value & 1)  // если комманда принимает аргументы, то нужно записать в машинный код её параметры
        {
            int information_bits = Write_Instruction_Arg(input_file_ptr, output_file_ptr, input_file_name, line);
            // first lowest bit = 1 if there is num, else 0
            // second lowest bit = 1 if there is register else 0
            // third bit = 1 if work with RAM, 0 if work with spu stack


            if (cmd_code_value & 0b10)   // может быть ситуация POP num, остальные ситуации для pop и push корректны
                Check_The_Correctness_of_Pop_Args(information_bits);

            //стоит также отметить, что jump команды принимают любой набор аргументов.
        } 
    }
    
    if (line == 0)
    {
        printf(ANSI_RED "\"%s\" is empty." ANSI_RESET_COLOR, input_file_name);
        return 1;        
    }
    return 0;
}

int pow(int x, int y)
{
    if (y == 0) return 1;
    return x * pow(x, y - 1);
}

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


// функция проверяет строку аргументов и переводит всё в машинный код
int Write_Instruction_Arg(FILE* input_file_ptr, FILE* output_file_ptr, const char *input_file_name, int curant_file_line)
{

    assert(input_file_ptr);
    assert(output_file_ptr);

    char args_str[64] = {};
    if (fscanf(input_file_ptr, "%s", args_str) <= 0)
    {
        printf(ANSI_RED "READING ERROR at %s:%d" ANSI_RESET_COLOR,input_file_name, curant_file_line);
        exit(1);
    } 



    unsigned information_about_arg_str = 0;
    // младший бит отвечает за наличие числа
    // второй бит отвечает за наличие регистра
    // третий бит отвечает за наличие открывающей скобки
    // четвёртый бит отвечает за наличие закрывающей скобки

    int num_arg = 0;
    int reg_arg = 0;
    
    int symbol_index = -1;
    while (args_str[symbol_index++] != '\0')
    {
        if (args_str[symbol_index] == '[')             
            information_about_arg_str |= 0b0100;
        else if (args_str[symbol_index] == ']')
            information_about_arg_str |= 0b1000;
        else if (args_str[symbol_index] <= 'Z' && args_str[symbol_index] >= 'A')
        {
            information_about_arg_str |= 0b0010;
            Check_The_Correctness_of_Register_Name(&args_str[symbol_index]);
            reg_arg = Find_Code_of_Register_By_Name(&args_str[symbol_index]);
            symbol_index++;  // т.к. след символ 'X', его нужно пропустить.
        }
        else if (args_str[symbol_index] <= '9' && args_str[symbol_index] >= '0')
        {
            information_about_arg_str |= 0b0001;

            STACK stk_num = {};
            Stack_Init(stk_num, 16);
            int len_of_num = 0;
            // разбираем число в обратном порядке, чтобы затем его преобразовать
            while (args_str[symbol_index] <= '9' && args_str[symbol_index] >= '0')
            {
                Stack_Push(stk_num, args_str[symbol_index] - '0');
                symbol_index++;
                len_of_num++;
            }
            num_arg = String_to_Int(stk_num, len_of_num);
            Stack_Destroy(stk_num);
            
        }
        else if (args_str[symbol_index] == ' ' || args_str[symbol_index] == '+')
        {
            continue;
        }
        else
        {
            printf("Unpredictable command: file_name=%s, line=%d\ncmd=%s\n", input_file_name, curant_file_line, args_str);
            exit(1);
        }
    }
    if ((information_about_arg_str & 0b1000) ^ (information_about_arg_str & 0b0100))
    {
        printf(ANSI_RED "Problem with brackets in command.cmd=%s" ANSI_RESET_COLOR, args_str);
        printf("information_about_arg_str=%d", information_about_arg_str);
        exit(1);
    }



    int information_bits = 0;
    // first lowest bit = 1 if there is num, else 0
    // second lowest bit = 1 if there is register else 0
    // third bit = 1 if work with RAM, 0 if work with spu stack

    information_bits |= information_about_arg_str & 0b111;

    // то же что и снизу только короче
    /*if (information_about_arg_str & 0b100) information_bits |= 0b100;
    if (information_about_arg_str & 0b10) information_bits |= 0b10;
    if (information_about_arg_str & 0b1) information_bits |= 0b1;*/
           
    fprintf(output_file_ptr, "%d\n", information_bits); //   записываем в машинный код информацию об аргументах (1-ый бит - есть ли чисто)
    if (information_bits & 0b10) fprintf(output_file_ptr, "%d\n", reg_arg);                              //  2-ой бит - есть ли регистр
    if (information_bits & 0b01) fprintf(output_file_ptr, "%d\n", num_arg);                              // 3-ий бит - есть ли обращение к ram
    
    return information_bits;
}



int Assembler(const char *input_file_name, const char *output_file_name)
{
    Read_File_and_Fill_Machine_Code(input_file_name, output_file_name);

    return 0;
}










// void Write_Various_Push_Situations_To_Code(SPU_t* spu_ptr, FILE* input_file_ptr)
// {

//     ON_DEBUG(assert(input_file_ptr); SPU_Assert(spu_ptr);)

//     char push_args[64] = {};
//         if (fscanf(input_file_ptr, "%s", push_args) <= 0)
//         {
//             printf(ANSI_RED "READING ERROR at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
//             exit(1);
//         } 
//     if (true)
//     {
//         char* register_name = (char *) calloc(16, sizeof(char));

//         ELEMENT_TYPE element_value = 0;
//         #warning existence of [ means nothing but existence of bit flag in the result
//         // отдельнеы биты для наличия числа и регистра (проходим посимвольно раз по команде). переменные для регистра и числа. далее if (flag && 001) code[ip++] = ......; отдельно проплюсить значения  переменной числа и регистра если бит равен 1
        
//         if (sscanf(push_args, "[%d]", &element_value) == 1)
//         {
//             spu_ptr->code[spu_ptr->size_code++] = ONLY_ELEMENT_PUSH_TO_RAM;
//             spu_ptr->code[spu_ptr->size_code++] = element_value;
//         }
//         else if (sscanf(push_args, "[%s]", register_name) == 1) 
//         {
//             spu_ptr->code[spu_ptr->size_code++] = ONLY_REGISTER_PUSH_TO_RAM;
//             spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
//         }
//         else if (sscanf(push_args, "[%s + %d]", register_name, &element_value) == 2) 
//             {
//                 spu_ptr->code[spu_ptr->size_code++] = REGISTER_AND_ELEMENT_PUSH_TO_RAM;
//                 spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
//                 spu_ptr->code[spu_ptr->size_code++] = element_value;
//             }
//         else if (sscanf(push_args, "%d", &element_value) == 1)
//         {
//             spu_ptr->code[spu_ptr->size_code++] = ONLY_ELEMENT_PUSH_TO_SPU_STACK;
//             spu_ptr->code[spu_ptr->size_code++] = element_value;
//         }
//         else if (sscanf(push_args, "%s", register_name) == 1) 
//         {
//             spu_ptr->code[spu_ptr->size_code++] = ONLY_REGISTER_PUSH_TO_SPU_STACK;
//             spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
//         }
//         else if (sscanf(push_args, "%s + %d", register_name, &element_value) == 2) 
//             {
//                 spu_ptr->code[spu_ptr->size_code++] = REGISTER_AND_ELEMENT_PUSH_TO_SPU_STACK;
//                 spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
//                 spu_ptr->code[spu_ptr->size_code++] = element_value;
//             }
//         else
//         {
//             printf(ANSI_RED "UNKNOWN COMAND \"%s\" at %s:%d(%s)" ANSI_RESET_COLOR, push_args, __FILE__, __LINE__, __FUNCTION__);
//             exit(1);
//         }

//         free(register_name);
//     }

// }


// то же самое что push, за исключением команды  "POP num". Т.о. 
// вызов wrtie;;.., затем проверка code (его последнего значение смотрим чтобы битик не был равен 1 который отвечает за переменное)на корректность 
// void Write_Various_Pop_Situations_To_Code(, FILE* input_file_ptr)
// {
//     ON_DEBUG(assert(input_file_ptr); SPU_Assert(spu_ptr);)

//     char pop_args[64] = {};
//         if (fscanf(input_file_ptr, "%s", pop_args) <= 0)
//         {
//             printf(ANSI_RED "READING ERROR at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
//             exit(1);
//         } 
    
//     char* register_name = (char *) calloc(16, sizeof(char));
    
//     ELEMENT_TYPE element_value = 0;
//     if (sscanf(pop_args, "[%d]", &element_value) == 1)
//     {
//         spu_ptr->code[spu_ptr->size_code++] = POP_ELEMENT_PUSH_TO_RAM;
//         spu_ptr->code[spu_ptr->size_code++] = element_value;
//     }    
//     else if (sscanf(pop_args, "[%s + %d]", register_name, &element_value) == 2) 
//         {
//             spu_ptr->code[spu_ptr->size_code++] = POP_ELEMENT_ADD_TO_REGISTER_PUSH_TO_RAM;
//             spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
//             spu_ptr->code[spu_ptr->size_code++] = element_value;
//         }
//     else if (sscanf(pop_args, "%s", register_name) == 1) 
//     {
//         spu_ptr->code[spu_ptr->size_code++] = POP_ELEMENT_PUSH_TO_REGISTERS;
//         spu_ptr->code[spu_ptr->size_code++] = Find_Code_of_Register_By_Name(register_name, __FILE__, __LINE__, __FUNCTION__);
//         #warning kakogo cherta
//     }
//     else
//     {
//         printf(ANSI_RED "UNKNOWN COMAND \"%s\" at %s:%d(%s)" ANSI_RESET_COLOR, pop_args, __FILE__, __LINE__, __FUNCTION__);
//         exit(1);
//     }

//     free(register_name);

// }


// // возможно стоит это запихать в структ уру
// // но это сложно инициализировать
// // но можно запихать в макрос
// // и это лучше чем пихать в макрос параметры