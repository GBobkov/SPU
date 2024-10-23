#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdlib.h>

enum RUNNER_ERROR
{
    NO_RUNNER_ERROR = 0,
    DIVISION_BY_ZERO=1
};


RUNNER_ERROR Run(int code[], size_t size);



#endif