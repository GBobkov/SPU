#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>

const char* input_name = "assembler.txt";
const char* output_name = "machine_code.txt";

int main()
{
    Assembler(input_name, output_name);
    
    printf("Done\n");
    return 0;
}