#include "stack_check.h"
const ELEMENT_TYPE POIZON_VALUE = 1917;
#ifdef DEBUG  
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>

const CANARY_t LST_CNR = 1111;
const CANARY_t RST_CNR = 1112;
const CANARY_t LDT_CNR = 1113;
const CANARY_t RDT_CNR = 1114;

uint64_t Calculate_HashSum(const char* start_byte, size_t size);
long long int Check_HashSum(STACK* stack);
long long int Check_Poizon(STACK* stack);
long long int Check_Canaries(STACK* stack);

CANARY_t* Calcualte_LDT_CANARY_ptr(STACK *stk);
CANARY_t* Calcualte_RDT_CANARY_ptr(STACK *stk);

CANARY_t* Calcualte_LDT_CANARY_ptr(STACK *stk)
{
    if (!stk)
        return NULL;
    return (CANARY_t *)((char *)stk->data - sizeof(CANARY_t));
}

CANARY_t* Calcualte_RDT_CANARY_ptr(STACK *stk)
{
    if (!stk)
        return NULL;
    return (CANARY_t *)((char *)stk->data + Calculate_Correct_Data_Size(stk->capacity, sizeof(ELEMENT_TYPE)));
}    

void Fill_Poizon(STACK *stack)
{
    for (int i = stack->size; i < stack->capacity; i++)
        stack->data[i] = POIZON_VALUE;
}


unsigned Stack_Error(STACK* stack)
{
    unsigned errors = 0;
    if (!stack) return NULL_STACK_PTR;
    if (stack->size < 0) errors |= NEGATIVE_SIZE;
    if (stack->capacity < 0) errors |= NEGATIVE_CAPACITY;
    if (stack->size > stack->capacity) errors |= SIZE_BIGGER_THAN_CAPACITY;
    if (!stack->data) errors |= NULL_DATA_PTR;
    errors |= Check_Damage(stack);
    stack->errors = errors;
    return errors;
}


int Do_Stack_Assert(STACK *stack, const char* file, const char* func, const int line)
{
    int i = 0;
    if (!stack)
    {
        printf("NULL_STACK_PTR in %s:%d(%s)", file, line, func);
        return NULL_STACK_PTR; 
    }
    if (Stack_Error(stack))
    {   
        Do_Stack_Dump(stack, file, func, line);
        return stack->errors;
    }
    return NO_ERROR;
}


void Do_Stack_Dump(STACK* stack, const char* file, const char* func, const int line)
{

    if (!stack)
    {
        
        printf("Assertion fault:NULL_STACK_PTR in %s:%d(%s)", file, line, func);
        return;   
    }
    printf(OPEN_LINE);
    printf(ANSI_PURPLE "STACK [%p]\n", stack);
    printf("\tname=\"%s\" initializated at %s:%d(%s)\n", stack->name, stack->file, stack->line, stack->func);
    printf("\tcalled from %s:%d(%s)" ANSI_RESET_COLOR, file, line, func);
    Printf_Errors(stack->errors);
    printf("capacity=%u\nsize=%u\n", stack->capacity, stack->size);
    printf("leftst_canary=%d\nrightst_canary=%d\n", stack->left_canary, stack->right_canary);
    printf("leftdt_canaty=%d\nrightdt_canary=%d\n", *Calcualte_LDT_CANARY_ptr(stack), *Calcualte_RDT_CANARY_ptr(stack));
    printf("struct_hashsum=%d\ndata_hashsum=%d\n", stack->struct_hashsum, stack->data_hashsum);
    printf(ANSI_YELLOW "data[%p]:" ANSI_RESET_COLOR, stack->data);
    if (!stack->data)
    {
        printf(ANSI_RED "Can't print data containing: NULL_DATA_PTR" ANSI_RESET_COLOR);
        printf(END_LINE);
        return;
    }
    printf(ANSI_BLACK " []=%ld(ldt_canary)" ANSI_RESET_COLOR, *Calcualte_LDT_CANARY_ptr(stack));
    for (int i = 0; i < (int)stack->capacity ; i++)
    {
        if (0 <= i && i < stack->size)
            printf(ANSI_GREEN "*[%d]=%d", i, stack->data[i]);
        else
            printf(ANSI_BLACK " [%d]=%d", i, stack->data[i]);

        if (stack->data[i] == POIZON_VALUE)
            printf(ANSI_RED"(POIZON)");

        printf(ANSI_RESET_COLOR);
    }
    printf(ANSI_BLACK " []=%ld(ldt_canary)" ANSI_RESET_COLOR, *Calcualte_RDT_CANARY_ptr(stack));
    if (!stack->capacity) printf(ANSI_RED "Stack is empty." ANSI_RESET_COLOR);
    printf(END_LINE);
}


void Printf_Errors(int problem)
{

    if (problem == 0)
    {
        printf(ANSI_GREEN "NO_ERRORS" ANSI_RESET_COLOR);
        return;
    }
    printf(ANSI_RED "ERRORS:");

    if (problem & NEGATIVE_SIZE) printf("\n\tNEGATIVE_SIZE");
    if (problem & NEGATIVE_CAPACITY) printf("\n\tNEGATIVE_CAPACITY");
    if (problem & SIZE_BIGGER_THAN_CAPACITY) printf("\n\tSIZE_BIGGER_THAN_CAPACITY");
    if (problem & NULL_STACK_PTR) printf("\n\tNULL_STACK_PTR");
    if (problem & NULL_DATA_PTR) printf("\n\tNULL_DATA_PTR");
    if (problem & LST_CNR_DEAD) printf("\n\tLST_CNR_DEAD");
    if (problem & RST_CNR_DEAD) printf("\n\tRST_CNR_DEAD");
    if (problem & LDT_CNR_DEAD) printf("\n\tLDT_CNR_DEAD");
    if (problem & RDT_CNR_DEAD) printf("\n\tRDT_CNR_DEAD");
    if (problem & DAMAGE_STRUCT_HASHSUM) printf("\n\tDAMAGE_STRUCT_HASHSUM");
    if (problem & DAMAGE_DATA_HASHSUM) printf("\n\tDAMAGE_DATA_HASHSUM");
    if (problem & UNPREDICTABLE_POIZON) printf("\n\tUNPREDICTABLE_POIZON");
    
    printf(ANSI_RESET_COLOR);

}


long long Check_Canaries(STACK* stack)
{                        
    long long int flag = 0;
    if (stack->left_canary != LST_CNR) 
        flag |= LST_CNR_DEAD;
        

    if (stack->right_canary != RST_CNR)
            flag |= RST_CNR_DEAD;

    if (*Calcualte_LDT_CANARY_ptr(stack) != LDT_CNR)
            flag |= LDT_CNR_DEAD;

    if (*Calcualte_RDT_CANARY_ptr(stack) != RDT_CNR)
            flag |= RDT_CNR_DEAD;
   
    return flag;
}


long long int Check_Damage(STACK* stack)
{
    long long int errors = Check_Canaries(stack);
    errors |= Check_HashSum(stack);
    errors |= Check_Poizon(stack);
    return errors;
}


uint64_t Calculate_HashSum(const char* start_byte, size_t size)
{
    uint64_t hashsum = 5381;
    if (size >= 0)
        for (size_t i = 0; i < size; i++)
            hashsum = (hashsum * 31) ^ start_byte[i];
    else
        for (size_t i = 0; i < size; i++)
            hashsum = (hashsum * 31) ^ start_byte[i];

    return hashsum;
}


void Update_Hashsums(STACK* stack)
{
    if (!stack || !stack->data)
    {
        printf(ANSI_RED "NULL_PTRS in %s:%d(%s)" ANSI_RESET_COLOR, __FILE__, __LINE__, __FUNCTION__);
        return;
    }
    stack->data_hashsum = Calculate_HashSum((const char *)Calcualte_LDT_CANARY_ptr(stack), Calcualte_RDT_CANARY_ptr(stack) - Calcualte_LDT_CANARY_ptr(stack) + 1);  
    stack->struct_hashsum = Calculate_HashSum((const char *) (&stack->struct_hashsum + 1), (char *)(&stack->right_canary) - (char *)(&stack->struct_hashsum + 1));
}


long long int Check_HashSum(STACK* stack)
{   
    if (!stack)
        return NULL_STACK_PTR;

    long long int error = 0;

    if (stack->struct_hashsum != Calculate_HashSum((const char *) (&stack->struct_hashsum + 1), (char *)(&stack->right_canary) - (char *)(&stack->struct_hashsum + 1)))
        error |= DAMAGE_STRUCT_HASHSUM;
    if (stack->data_hashsum != Calculate_HashSum((const char *)Calcualte_LDT_CANARY_ptr(stack), Calcualte_RDT_CANARY_ptr(stack) - Calcualte_LDT_CANARY_ptr(stack) + 1))
        error |= DAMAGE_DATA_HASHSUM;

    return error;
}


long long int Check_Poizon(STACK* stack)
{
    if (!stack)
        return NULL_STACK_PTR;
    if (!stack->data)
        return NULL_DATA_PTR;

    for (int i = 0; i < stack->size; i++)
        if (stack->data[i] == POIZON_VALUE)
            return UNPREDICTABLE_POIZON;
        
    for (int i = stack->size; i < stack->capacity; i++)
        if (stack->data[i] != POIZON_VALUE)
            return UNPREDICTABLE_POIZON;
    return NO_ERROR;
}


uint64_t Calculate_Correct_Data_Size(size_t elements_num, size_t element_size)
{
    int delta_bytes = ((8 - element_size * elements_num) % 8);
    return elements_num * element_size +  delta_bytes;
}  

#endif