#ifndef ENUM_H
#define ENUM_H

enum COMANDS
{
    // the lowest two bit contain information about variable.
    // the first lowest bit =0 if this command don't have args 0
    //                      =1 if this command has any arguments. 
    // the second lowest bit=0 if this command read args from file
    //                      =1 if this command write to args  (this bit will be considered if the first lowest bit=1).
    // (only CMD_POP) other command may have 1 or 0 in the second
    // lowest bit

    CMD_PUSH    = 0b000001,
    CMD_POP     = 0b000011,
    CMD_ADD     = 0b000010,
    CMD_SUB     = 0b000100,
    CMD_MUL     = 0b000110,
    CMD_DIV     = 0b001000,
    CMD_OUT     = 0b001010, 
    CMD_IN      = 0b001100,
    CMD_DUMP    = 0b001110,
    CMD_JA      = 0b010001, // 5 бит=1, если это команда типа jump
    CMD_JAE     = 0b010011,
    CMD_JB      = 0b010101,
    CMD_JBE     = 0b010111,
    CMD_JE      = 0b011001,
    CMD_JNE     = 0b011011,
    CMD_JMP     = 0b011101,
    CMD_RET     = 0b100000,
    CMD_CALL    = 0b100001,
    CMD_HLT     = 0
};

enum BITS_VALUE
{
    NUMBER_BIT          = 0b0001,
    REGISTER_BIT        = 0b0010,
    OPEN_BRACKET_BIT    = 0b0100,
    CLOSE_BRACKET_BIT   = 0b1000
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


#endif