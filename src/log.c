#include "log.h"
#include <stdio.h>

void printFile(uint8* bin, size_t size, int option){

    switch (option) {
        case(BINARY):{
            for (int i = 0; i < size; i++){
                printf("%i: %b\n",i, GETBIT_8(bin[i]));
            }
        }   break;

        case(OCTAL):{
            for (int i = 0; i < size; i++){
                printf("%o ", GETBIT_8(bin[i]));
            }
        }   break;

        case(DECIMAL):{
            for (int i = 0; i < size; i++){
                printf("%d ", GETBIT_8(bin[i]));
            }
        }   break;

        
        case(HEX):{
            for (int i = 0; i < size; i++){
                printf("%X ", GETBIT_8(bin[i]));
            }
        }   break; 
    }
    printf("\n");
}


void printBinFile(BinFile f){
    LOG_STRING("Content: ");
    for(int i = 0; i < f.size; i++){
        printf("%c", f.content[i]);
    }
    LOG_STRING("\n");
}

