#include "command_argument_handler.h"


#include "enum.h"
#include "my_stack.h"
#include "colors.h"
#include <assert.h>


#define BIN_OUTPUT_MODE 0

#if BIN_OUTPUT_MODE == 0
#define FPRINTF_NUMBER_IN_OUTPUT(number) fprintf(output_file_ptr, "%d\n", number);
#else
#define FPRINTF_NUMBER_IN_OUTPUT(number) const int BUF_SIZE = 8;\
        {\
            char buffer[BUF_SIZE];\
            buffer[BUF_SIZE - 1] = '\0';\
            Int_to_Bin(number, buffer, BUF_SIZE - 1);\
            fprintf(output_file_ptr, "%s\n", buffer);
        }
#endif


//возведение в степень
static int pow(int x, int y)
{
    if (y == 0) return 1;
    return x * pow(x, y - 1);
}



// перевести строку в число (atoi) 
static int Str_to_Int(STACK stk, int len_of_num)
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


// Функция возвращает код ошибки. Считывает число со строки
static int Sscanf_Number(const char* start_sym, int *symbol_index, int *output_variable, int sign)
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
    *output_variable = Str_to_Int(stk_num, len_of_num) * sign;
    Stack_Destroy(stk_num);

    *symbol_index += len_of_num - 1;     // -1 т.к. мы должны переместить указатель на конец числа, а не на след-ий символ после числа.
    return 0;               
}



// проверка корректности имени регистра
static int Check_Reg_Name(const char *fst_sym)
{
    // Первый символ - A..E. Второй символ = 'X'. Следующим символом после регистра должен быть ' ' || '+' || '-' || '\0' || ']'.
    const char HIGHT_REG_SYM = 'E';

    if ((fst_sym[1] != 'X') || (fst_sym[2] != ' ' &&  fst_sym[2] != '+' && fst_sym[2] != '\0' && fst_sym[2] != ']') || (fst_sym[0] > HIGHT_REG_SYM))
    {
        printf("incorrect register_name. args_str=[%s", fst_sym);
        abort();
    }

    return 0;
}


// функция возвращает номер регистра
static int Find_Reg_Code_By_Name(const char* reg_name)
{
    // AX = 1 = 'A' - 'A'  + 1
    // BX = 2 = 'B' - 'A'  + 1
    // ...
    return reg_name[0] - 'A' + 1; // + 1, т.к. нулевой регистр ошибочный
}


// Возвращает код ошибки. Функция проверяет корректность расстановки квадратных скобок.
static int Check_Brackets(unsigned str_arg_info_bits,  const char *args_str)
{
    // рассмотрим 3,4 биты отвечающие за наличие квадратных скобок
    if (((str_arg_info_bits & CLOSE_BRACKET_BIT) >> 1) ^ (str_arg_info_bits & OPEN_BRACKET_BIT)) // сдвиг вправо на 1 бит, чтобы биты наложились
    {
        printf(ANSI_RED "Problem with brackets in command.args=%s" ANSI_RESET_COLOR, args_str);
        abort();
    }
    return 0;
}



// Проверка, что знак выставлен.
static int Check_Sign(int sign, const char* args_str)
{
    if (!sign)
    {
        printf(ANSI_RED "There is no sign! args=\"%s\"" ANSI_RESET_COLOR, args_str);
        abort();
    }

    return 0;
}


// Возвращает 1 если ошибка в чтении аргументов. 
int Read_Arg_Line(const char* args_str, unsigned *str_arg_info_bits, int* num_arg, int* reg_arg)
{
    int sign = 1;  // 1 <=> +;    -1 <=> -;
    int symbol_index = -1;
    while (args_str[++symbol_index] != '\0')
    {
        if (args_str[symbol_index] == '[')             
            *str_arg_info_bits |= OPEN_BRACKET_BIT;
        else if (args_str[symbol_index] == ']')
            *str_arg_info_bits |= CLOSE_BRACKET_BIT;
        else if (args_str[symbol_index] <= 'Z' && args_str[symbol_index] >= 'A')
        {   
            Check_Sign(sign, args_str);  // аргумент должен быть после знака
            *str_arg_info_bits |= REGISTER_BIT;
            Check_Reg_Name(&args_str[symbol_index]);
            *reg_arg = Find_Reg_Code_By_Name(&args_str[symbol_index]) * sign;
            symbol_index++;  // т.к. след символ 'X', его нужно пропустить.

            sign = 0; // т.к. операция прошла.
        }
        else if (args_str[symbol_index] <= '9' && args_str[symbol_index] >= '0')
        {  
            Check_Sign(sign, args_str);  // аргумент должен быть после знака
            *str_arg_info_bits |= NUMBER_BIT;
            Sscanf_Number(&args_str[symbol_index], &symbol_index, num_arg, sign);
            
            sign = 0; // т.к. операция прошла.
        }
        else if (args_str[symbol_index] == '+')
        {
            sign = +1;
        }
        else if (args_str[symbol_index] == '-')
        {
            sign = -1;
        }
        else if (args_str[symbol_index] == ' ' || args_str[symbol_index] == '\n')
        {
            continue;
        }
        else
            return 1;
    }
    return 0;
}


// функция проверяет строку аргументов и переводит всё в машинный код. Возвращает  str_arg_info_bits.
int Write_Instruction_Arg(FILE* input_file_ptr, STACK* commands_stack_ptr, const char *input_file_name, int current_file_line, LABEL* lbls)
{   
    assert(input_file_ptr); 

    char args_str[64] = {};
    if (fscanf(input_file_ptr, "%s", args_str) <= 0)
    {
        printf(ANSI_RED "READING ERROR at %s:%d" ANSI_RESET_COLOR,input_file_name, current_file_line);
        abort();
    } 

    unsigned str_arg_info_bits = 0;
    // младший бит отвечает за наличие числа
    // второй бит отвечает за наличие регистра
    // третий бит отвечает за наличие открывающей скобки  [
    // четвёртый бит отвечает за наличие закрывающей скобки  ]

    int num_arg = 0;
    int reg_arg = 0;
    // поиск : в строке, проверка что : последний символ. подстановка метки. return 0b1000 - 4-ый бит отвечает за метку. метка может быть только у jump-cmd.
    if (Is_Label_Name(args_str))
    {
        str_arg_info_bits |= NUMBER_BIT;
        num_arg = Search_Index_Code_By_Label(lbls, args_str);
        if (num_arg == -1)
        {
            printf(ANSI_RED "Can't find label=\"%s\"" ANSI_RESET_COLOR, args_str);
            abort();
        }
    }
    else 
    {
        if (Read_Arg_Line(args_str, &str_arg_info_bits, &num_arg, &reg_arg))
        {
            printf(ANSI_RED "Unpredictable command: file_name=%s, current_file_line=%d\ncmd=%s" ANSI_RESET_COLOR, input_file_name, current_file_line, args_str);
            abort();
        }
        Check_Brackets(str_arg_info_bits, args_str);
    }
    
    str_arg_info_bits &= 0b111;           
    //1-ый бит - есть ли чисто
    //2-ой бит - есть ли регистр
    //3-ий бит - есть ли обращение к ram

    // записываем в машинный код информацию об аргументах
    Stack_Push(*commands_stack_ptr, str_arg_info_bits);
    if (str_arg_info_bits & NUMBER_BIT)   //1-ый бит - есть ли чисто
        Stack_Push(*commands_stack_ptr, num_arg);
    if (str_arg_info_bits & REGISTER_BIT)   //2-ой бит - есть ли регистр
        Stack_Push(*commands_stack_ptr, reg_arg);

    return str_arg_info_bits;
}