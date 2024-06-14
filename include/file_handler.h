#ifndef FILE_HANDLER_H_
#define FILE_HANDLER_H_
#include <stdint.h>
#include <stdio.h>

typedef uint8_t uint8;
typedef uint16_t uint16;

typedef int8_t int8 ;
typedef int16_t int16 ;

typedef struct {
    FILE* fptr;
    size_t size;
    const char* path;
    uint8 *content;
} BinFile;

BinFile* open_binfile(const char*);

uint8* load_file_into_memory(
        int,
        size_t
);

#endif
