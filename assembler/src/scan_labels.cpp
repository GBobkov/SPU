#include "scan_labels.h"

#include "enum.h"
#include "command_argument_handler.h"
#include "colors.h"
#include <stdio.h> 
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static const int AMOUT_OF_LABELS = 20;

// Создает Пустую метку 
static LABEL Create_Label(void)
{
    LABEL lbl = {};
    memset((void *) lbl.label_name, 0, 16);
    lbl.label_code_position = -1;
    lbl.label_line_position_first_meet = -1;
    return lbl;
}


// создаём массив меток
LABEL* Create_Labels_Arr(void)
{
    LABEL* lbls = (LABEL *) calloc(AMOUT_OF_LABELS, sizeof(LABEL));
    for (int i = 0; i < AMOUT_OF_LABELS; i++)
        lbls[i] = Create_Label();
    return lbls;
    //calloc
    //return 
}


// функиця уничтожает массив
int Destroy_Labels_Arr(LABEL *lbls)
{
    free(lbls);
    return 0; 
}


// функция находит номер ip в code, где была объявленна данная метка. если метка не была объявлена то возвращает -1.
int Search_Index_Code_By_Label(LABEL* lbls, const char* lbl_name)
{
    for (int i = 0; i < AMOUT_OF_LABELS; i++)
    {   
        if (lbls[i].label_name[0] == '\0')   // означает что мы прошли все известные метки и не нашли нужную. 
        {                                   
            return -1;
        }
        if (strcmp(lbls[i].label_name, lbl_name) == 0)
            return lbls[i].label_code_position;
    }

    return 0;
}



// Функция записывает метку и индекс её направления в массив меток. Возвращает код ошибки.
static int Write_New_Label_to_Arr(LABEL* lbls, const char* lbl_name, int code_ip, int input_file_line)
{
    for (int i = 0; i < AMOUT_OF_LABELS; i++)
    {   
        if (lbls[i].label_name[0] == '\0')   // означает что мы прошли все известные метки и не нашли нужную. Запишем её имя и индекс в массив.
        {
            strcpy(lbls[i].label_name,lbl_name);
            lbls[i].label_code_position = code_ip;
            lbls[i].label_line_position_first_meet = input_file_line;
            return 0; 
        }
        if (strcmp(lbls[i].label_name, lbl_name) == 0)     // Если имя метки нашлось.
        {                                                   // Если метка не объявлена инициализируем её.
            if (lbls[i].label_code_position == -1) 
            {
                lbls[i].label_code_position = code_ip; 

                // Если метка встретилась впервые тогда, когда её объявляют.
                if (lbls[i].label_line_position_first_meet == -1) lbls[i].label_line_position_first_meet = input_file_line;

                return 0;
            }
            else // Если метка уже объявлена то проверим, это метка из аргумента или нет (считаем, что метка объявляется один раз.)
            {
                if (code_ip == -1) return 0; // значит метка находится в аргументе, что допускается.

                // иначе: повторное объявление метки.
                printf(ANSI_RED "Error: Label was declarated twice in line=%d." ANSI_RESET_COLOR, input_file_line);
                abort();
            }
        } 
    }

    printf(ANSI_RED "Возможно, массив меток переполнен. Problem at %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
    return 1;
}


// возвращает код ошибки. функция перемещает instr_ptr в зависимости от кол-ва аргументов
static int Shift_Instruction_Pointer(int *instr_ptr, int information_bits)
{
    int delta_ip = 0;
    // information_bits - значение битов информации о параметрах 
    delta_ip += 1;  // т.к. необходимо сдвинуться вправо на 1 из-за ячейки с информацией о переменной
    delta_ip += information_bits & NUMBER_BIT; // сдвиг на 1 вправо если есть переменная = числу
    delta_ip += (information_bits & REGISTER_BIT) >> 1; // сдвиг на 1 вправо если есть переменная = регистру

    *instr_ptr += delta_ip;
    return 0;
}

// Проверка на то, что имя метки только из букв
static bool Is_Correctness_of_Variable_Name_Okey(const char* cmd)
{
    bool okey = isalpha(cmd[0]);
    int pointer = 1;
    while (cmd[pointer] != ':' && okey)
    {
        okey &= isalpha(cmd[pointer]) || isdigit(cmd[pointer]) || (cmd[pointer] == '_');
        pointer++;
    }
    return okey;
}   



// функция проверит корректность имени метки. true если это имя метки, иначе false.
bool Is_Label_Name(const char* cmd)
{
    
    char *colon_position = strchr(cmd, ':');
    if (!colon_position) // не нашлось ':'. Значит это неизвестная команда
        return false;
    if (*(colon_position + 1) != '\0' && *(colon_position + 1) != ' ') // двоеточие не в конце имени
        return false;
    if (!Is_Correctness_of_Variable_Name_Okey(cmd))
        return false;
    return true; // имя метки удовлетворяет требованиям
}


// static void Print_Label_Name_Errors(int errors, int input_file_line, const char* cmd)
// {
//     if (errors & NUMBER_BIT) printf(ANSI_RED "Unsupportable arguments in input_file_line=%d. Arg_str=%s\n", input_file_line, cmd);
//     if (errors & REGISTER_BIT) printf(ANSI_RED "Wrong colon position in label_name in input_file_line=%d. Arg_str=%s\n", input_file_line, cmd);
//     if (errors & OPEN_BRACKET_BIT) printf(ANSI_RED "Wrong label_name in input_file_line=%d. Arg_str=%s" ANSI_RESET_COLOR, input_file_line, cmd);
// }



// Проверяет, все ли метки объявлены(ситуация когда метка встретилась в аргументах, но не встретилась в объявлении).
// Возвращает код ошибки.
static bool Is_Lables_Arr_Okay(LABEL* lbls)
{
    bool okey = true;
    for (int i = 0; i < AMOUT_OF_LABELS && lbls[i].label_name[0] != '\0'; i++)
        if (lbls[i].label_code_position == -1)
        {
            printf(ANSI_RED "undeclared label at line=%d. label_name=%s" ANSI_RESET_COLOR, lbls[i].label_line_position_first_meet, lbls[i].label_name);
            okey = false;
        }
    
    return okey;
}

// Принимает массив меток(пустой) и заполняет его информацией
int Scan_File_Search_Labels(LABEL* labels, const char* input_file_name)
{
    FILE* input_file_ptr = fopen(input_file_name, "r");
    
    char cmd[50] = {};
    int instr_ptr = 0;
    int input_file_line = 0;
    while (fscanf(input_file_ptr, "%s", cmd) > 0)
    {
        input_file_line++; // след строка.
        instr_ptr++; // т.к. мы прочитали имя команды. Далее стоит учесть, что если прочитанная команда - метка, то "instr_ptr--", 
                    //  из-за того, что метка не занимает место в машинном коде.

        
        // если комманда без аргументов, то переходим дальше.
        #define CODEGEN(name) \
            if (strcmp(cmd, #name) == 0) {continue;} \
                else

        CODEGEN(ADD)
        CODEGEN(SUB)
        CODEGEN(MUL)
        CODEGEN(DIV)
        CODEGEN(OUT)
        CODEGEN(IN)
        CODEGEN(DUMP)
        CODEGEN(RET)
        CODEGEN(HLT)
        #undef CODEGEN

        // если PUSH, POP
        // нам не нужная информация о том, какое значение у арг-ов.
        // важно только их количество, чтобы верно сдвинуть instr_ptr.
        #define CODEGEN(name) \
            if (strcmp(cmd, #name) == 0) \
            { \
                int unnecessary_var = 0;\
                unsigned information_bits = 0;\
                char arg_str[50] = {};\
                fscanf(input_file_ptr, "%s", arg_str);\
                if(Read_Arg_Line(arg_str, &information_bits, &unnecessary_var, &unnecessary_var))\
                {\
                    printf(ANSI_RED "Wrong arguments in input_file_line=%d. arg_str=%s" ANSI_RESET_COLOR, input_file_line, arg_str);\
                    abort();\
                }\
                Shift_Instruction_Pointer(&instr_ptr, information_bits);\
            }\
            else

        CODEGEN(PUSH)
        CODEGEN(POP)
        /*else*/ 
        {
            #undef CODEGEN

            // Для jump-комманд
            #define CODEGEN(name) \
                if (strcmp(cmd, #name) == 0) \
                { \
                    int unnecessary_var = 0;\
                    unsigned information_bits = 0;\
                    char arg_str[50] = {};\
                    fscanf(input_file_ptr, "%s", arg_str);\
                    if (!Is_Label_Name(arg_str))\
                    {\
                        if(Read_Arg_Line(arg_str, &information_bits, &unnecessary_var, &unnecessary_var))\
                        {\
                            printf(ANSI_RED "Line=%d. Unsuportable args_str=%s" ANSI_RESET_COLOR, input_file_line, arg_str);\
                            abort();\
                        }\
                        Shift_Instruction_Pointer(&instr_ptr, information_bits);\
                    }\
                    else\
                    {\
                        Shift_Instruction_Pointer(&instr_ptr, NUMBER_BIT);\
                        Write_New_Label_to_Arr(labels, arg_str, -1, input_file_line);\
                    }\
                }\
                else

            CODEGEN(JA)
            CODEGEN(JAE)
            CODEGEN(JB)
            CODEGEN(JBE)
            CODEGEN(JE)
            CODEGEN(JNE)
            CODEGEN(JMP)
            CODEGEN(CALL)
            /*else*/
            {
                #undef CODEGEN

                if (strcmp(cmd, "CALL") == 0)
                {
                    char arg_str[50] = {};
                    fscanf(input_file_ptr, "%s", arg_str);
                    if (!Is_Label_Name(arg_str))
                    {
                        printf(ANSI_RED "CALL accepts only labels. line=%d. arg_str=%s" ANSI_RESET_COLOR, input_file_line, arg_str);
                        abort();
                    }
                    else
                    {
                        Shift_Instruction_Pointer(&instr_ptr, NUMBER_BIT); // т.к. комманда call имеет всегда только один аргумент - номер code_ip.
                        Write_New_Label_to_Arr(labels, arg_str, -1, input_file_line);
                    }
                    continue;
                }


                // Т.О данная комманда не является стандартной, а значит это или метка, или ошибочная комманда.

                // Проверим, что это метка. Если это не так, то прерываем программу.
                if (!Is_Label_Name(cmd))
                {
                    printf(ANSI_RED "Line=%d. Unsuportable command=%s" ANSI_RESET_COLOR, input_file_line, cmd);
                    abort();
                }

                // Если это метка, то она объявляет строку.
                instr_ptr--; // чтобы понять "--": см. начало цикла while.
                Write_New_Label_to_Arr(labels, cmd, instr_ptr, input_file_line);
            }
        }
    }

    if (!Is_Lables_Arr_Okay(labels)) 
    {
        //printf(ANSI_RED "Lables arr is wrong!" ANSI_RESET_COLOR);
        // все printf-ы в функции Is_Lables_Arr_Okay.
        abort();
    }

    fclose(input_file_ptr);
    return 0;
    
}



// далее переход в асм.спп в ф Write_Instruction_Arg