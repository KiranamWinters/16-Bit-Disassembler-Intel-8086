#include "file_handler.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>

uint8* load_file_into_memory(int fd,size_t size){
    uint8* ptr_to_file_in_memory = (uint8*)mmap(
            NULL,
            size  ,
            PROT_READ,
            MAP_PRIVATE,
            fd,
            0
    );

    return ptr_to_file_in_memory;
}

BinFile* open_binfile(const char* path_to_file){
    BinFile *result = malloc(sizeof(BinFile));
    struct stat st;
    stat(path_to_file, &st);
    result->size = st.st_size;
    result->path = path_to_file;
    result->fptr = fopen(path_to_file, "r");
    if (result->fptr == NULL){
        printf("ERROR: Could not open file\n");
    }
    int fd = fileno(result->fptr);
    result->content = load_file_into_memory(fd, st.st_size);
     
    return result;
}


