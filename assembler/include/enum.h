enum COMANDS
{
    // the lowest two bit contain information about variable
    // the first lowest bit=1 if this command have
    // some arguments if this command don't have args 0
    // the second lowest bit=0 if this command read args from 
    // file(only CMD_PUSH) and =1 if this command write to args
    // (only CMD_POP) other command may have 1 or 0 in the second
    // lowest bit

    CMD_PUSH= 0b01,
    CMD_POP = 0b11,
    CMD_ADD = 0b10,
    CMD_SUB = 0b100,
    CMD_MUL = 0b110,
    CMD_DIV = 0b1000,
    CMD_OUT = 0b1010, 
    CMD_IN  = 0b1100,
    CMD_DUMP= 0b1110,
    CMD_JA  = 0b10001, 
    CMD_JAE = 0b10011,
    CMD_JB  = 0b10101,
    CMD_JBE = 0b10111,
    CMD_JE  = 0b11101,
    CMD_JNE = 0b11111,
    CMD_JMP = 0b100001,
    CMD_HLT = 0
};


enum REGISTERS_CODES
{
    UNDEFINED_REGISTER_CODE = 0,
    REGISTER_NAME_AX = 1,
    REGISTER_NAME_BX,
    REGISTER_NAME_CX,
    REGISTER_NAME_DX,
    REGISTER_NAME_EX
};