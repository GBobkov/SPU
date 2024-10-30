#include "assembler.h"
#include "processor.h"
#include <stdio.h>
#include <stdlib.h>

const char* input_name = "assembler.asm";
const char* output_name = "machine_code.bin";



int main()
{
    Assembler(input_name, output_name);
    Run(output_name);
    printf("Done\n");
    return 0;
}