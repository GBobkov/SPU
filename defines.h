#ifndef ALL_DEFINES_H
#define ALL_DEFINES_H

#define DEBUG

#ifdef DEBUG 
#define ON_DEBUG(code) code
#else 
#define ON_DEBUG(code) 
#endif



#define Stack_Init(stk, capacity) Do_Stack_Init(&stk, capacity, #stk, __FILE__,__FUNCTION__, __LINE__)
#define Stack_Push(stk, element) Do_Stack_Push(&stk, element)
#define Stack_Pop(stk) Do_Stack_Pop(&stk)
#define Stack_Dump(stk) Do_Stack_Dump(&stk, __FILE__, __FUNCTION__, __LINE__)
#define Stack_Assert(stk) Do_Stack_Assert(stk, __FILE__, __FUNCTION__, __LINE__)
#define Stack_Destroy(stk) Do_Stack_Destroy(&stk, #stk, __FILE__,__FUNCTION__, __LINE__)




#endif