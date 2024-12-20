#ifndef COMMAND_ARGUMENT_HANDLER_H
#define COMMAND_ARGUMENT_HANDLER_H

#include "my_stack.h"
#include "scan_labels.h"
#include <stdio.h>

// функция обрабатывает строку аргументов и переводит всё в машинный код 
int Write_Instruction_Arg(FILE* input_file_ptr, STACK* commands_stack, const char *input_file_name, int current_file_line, LABEL* lbls);


// Возвращает код ошибки. Считывает аргументы строки. Заполняет биты информации.
int Read_Arg_Line(const char* args_str, unsigned *str_args_information_bits, int* num_arg, int* reg_arg);

#endif