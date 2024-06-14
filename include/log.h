#ifndef LOG_H_
#define LOG_H_

#include "disassembler.h"
#include "file_handler.h"
void printBinFile(BinFile);
void printFile(uint8* , size_t, int);

#define LOG_STRING(src) printf("%s",src)
#define LOG_INT(src) printf("%d", src)
#define BINARY 0x01
#define OCTAL 0x02
#define DECIMAL 0x03
#define HEX 0x04
#endif
