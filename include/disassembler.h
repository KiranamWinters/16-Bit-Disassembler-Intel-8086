#ifndef DISASSEMBLER_H_ 
#define DISASSEMBLER_H_

#include "file_handler.h"

#define FALSE 0

#define GET_OPCODE(x,n) ( n == 4 ) ? ((0b11110000 & x) >> 4) : ((0b11111100 & x)>>2)
#define GET_OPCODE_SIXBIT(x) GET_OPCODE(x,6)

#define GETBIT_8(x) (0xFF & x)

#define GET_BIT(x,offset) (((0b10000000 >> offset) & x) >> (7 - offset))
#define GET_REG(x,offset) (((0b11100000 >> offset) & x) >> (5 - offset))
#define GET_RM(x,offset) GET_REG(x,offset)

#define GET_MOD(x,offset) ((0b11000000 >> offset) & x) >> (6 - offset)
#define GET_SR(x,offset) GET_MOD(x, offset)
void disassemble(BinFile*);
    
#endif 
