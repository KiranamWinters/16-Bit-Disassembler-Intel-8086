#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "disassembler.h"

#include "file_handler.h"
int main(int argc, char *argv[])
{
    BinFile *src = open_binfile(argv[1]);
    #ifdef DEBUG
    printf("File Pointer: %X\n", src.fptr);
    printf("File Path: %s\n", src.path);
    printf("File Size: %d\n", src.size);
    #endif


    disassemble(src);
    return 0;
}
