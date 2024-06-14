#include "disassembler.h"
#include "file_handler.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

//REGISTER LOOKUP TABLE
const char* REG[] = {
    //W: 0 | 8-Bit Registers
    "al",   //000 
    "cl",   //001 
    "dl",   //010
    "bl",   //011
    "ah",   //100
    "ch",   //101
    "dh",   //110
    "bh",   //111

    //W: 1 | 16-bit REGISTERS
    "ax",   //000 + 8
    "cx",   //001 + 8
    "dx",   //010 + 8
    "bx",   //011 + 8
    "sp",   //100 + 8
    "bp",   //101 + 8
    "si",   //110 + 8
    "di"    //111 + 8 
};

//SEGEMENT REGISTERS
const char* SEGMENT_REGISTER[] = {
    "es",
    "cs",
    "ss",
    "ds"
};

const char* get_reg(uint8 src, uint8 offset, uint8 w){
    uint8 w_offset = (w) ? 8: 0;
    return REG[GET_REG(src, offset) + w_offset];
}

char* get_reg_or_memory(uint8 *src, size_t index, uint8 offset, uint8 w){
    char* buffer = malloc(sizeof(char) * 15);

    switch (GET_MOD(src[index+1], 0)) {
        case(0b00) :{
            switch(GET_RM(src[index+1], offset))  {
                case(0b000):{strcpy(buffer,"[bx + si]") ;}          break;
                case(0b001):{strcpy(buffer,"[bx + di]") ;}          break;
                case(0b010):{strcpy(buffer,"[bp + si]") ;}          break;
                case(0b011):{strcpy(buffer,"[bp + di]") ;}          break;
                case(0b100):{strcpy(buffer,"[si]")      ;}          break;
                case(0b101):{strcpy(buffer,"[di]")      ;}          break;
                case(0b111):{strcpy(buffer,"[bx]")      ;}          break;
                case(0b110):{
                    sprintf(buffer,"[%d]", (src[index + 3] << 8) | src[index + 2]);  
                }   break;
            }
        }       break;

        case(0b01) :{
            switch (GET_RM(src[index+1], offset)) {
                case(0b000):{sprintf(buffer,"[bx + si + %d]",src[index+2]);}         break;
                case(0b001):{sprintf(buffer,"[bx + di + %d]",src[index+2]);}         break;
                case(0b010):{sprintf(buffer,"[bp + si + %d]",src[index+2]);}         break;
                case(0b011):{sprintf(buffer,"[bp + di + %d]",src[index+2]);}         break;
                case(0b100):{sprintf(buffer,"[si + %d]"     ,src[index+2]);}         break;
                case(0b101):{sprintf(buffer,"[di + %d]"     ,src[index+2]);}         break;
                case(0b111):{sprintf(buffer,"[bx + %d]"     ,src[index+2]);}         break;
                case(0b110):{
                    (src[index + 2]) ? sprintf(buffer,"[bp + %d]",src[index+2]) : sprintf(buffer,"[bp]");    
                }   break;
            }
        }       break;

        case(0b10) :{
            switch (GET_RM(src[index+1], offset)) {
                   
                case(0b000):{sprintf(buffer,"[bx + si + %d]",((src[index+3] << 8) | src[index + 2])); }         break;
                case(0b001):{sprintf(buffer,"[bx + di + %d]",((src[index+3] << 8) | src[index + 2])); }         break;
                case(0b010):{sprintf(buffer,"[bp + si + %d]",((src[index+3] << 8) | src[index + 2])); }         break;
                case(0b011):{sprintf(buffer,"[bp + di + %d]",((src[index+3] << 8) | src[index + 2])); }         break;
                case(0b100):{sprintf(buffer,"[si + %d]"     ,((src[index+3] << 8) | src[index + 2])); }         break;
                case(0b101):{sprintf(buffer,"[di + %d]"     ,((src[index+3] << 8) | src[index + 2])); }         break;
                case(0b110):{sprintf(buffer,"[bp + %d]"     ,((src[index+3] << 8) | src[index + 2])); }         break;
                case(0b111):{sprintf(buffer,"[bx + %d]"     ,((src[index+3] << 8) | src[index + 2])); }         break; 
            }
        }       break;
        
        case(0b11) :{strcpy(buffer, get_reg(src[index + 1], 5, w));} break; 
    }
    return buffer;
}

const char* get_opcode(uint8 bin){   
    switch (GET_OPCODE(bin, 6)){
            
        case(0b100010):{return "mov";}      break;  //
        case(0b101000):{return "mov";}      break;
        case(0b110001):{return "mov";}      break;
        case(0b100011):{return "mov";}      break;

        default:{

            if((GET_OPCODE(bin, 4)) == 0b1011){
                return "mov";
            }

            return "OPCODE_NOT_AVAILABLE";
        }   break;
    }
    return "NONE";
}

int mod_based_increment(uint8 MOD, uint8 RM){
    switch(MOD){
        case(0b00):{
            if(RM == 0b110) {return 4;}            
            else            {return 2;}
        }   break;

        case(0b01):{return 3;}    break;
        case(0b10):{return 4;}    break;
        case(0b11):{return 2;}    break;
    }
    return -1;
}


void disassemble(BinFile* file){
    uint8* bin = file->content;
    printf("bits 16\n\n"); 

    int i = 0;
    while(i + 1 < file->size ){

        switch(GET_OPCODE(bin[i], 6)){

            //INSTRUCTION(MOV):  register/memory to/from register
            case(0b100010):{

                uint8 d_bit = GET_BIT(bin[i], 6);
                uint8 w_bit = GET_BIT(bin[i], 7);

                const char *src;
                const char *dest;
                
                char* temp = get_reg_or_memory(bin,i,5,w_bit); 
                
                if(d_bit){
                    src  =  temp;
                    dest =  get_reg(bin[i+1],2,w_bit);
                }else{
                    dest =  temp; 
                    src  =  get_reg(bin[i+1],2,w_bit);
                }

                printf("%s %s, %s\n",get_opcode(bin[i]),dest, src);
                free(temp);

                i += mod_based_increment(GET_MOD(bin[i+1],0), GET_RM(bin[i+1], 5));
            }   break;

            //INSTRUCTION(MOV): MEMORY/ACCUMULATOR TO ACCUMULATOR/MEMORY  
            case(0b101000):{
                if(!GET_BIT(bin[i], 6)){
                    printf("%s %s, [%d]\n",get_opcode(bin[i]),"ax",(bin [i+2] << 8)|bin[i+1]);
                }else{
                    printf("%s [%d], %s\n",get_opcode(bin[i]),bin[i+2] << 8 | bin[i+1],"ax");
                }
                i = i+3;
            }   break;
            
            //INSTRUCTION(MOV): IMMEDIATE TO REGISTER/MEMORY
            case(0b110001):{
                //TO BE DONE
                i = i+2;
            }   break;

            //INSTRUCTION(MOV): SEGMENT_REGISTER
            case(0b100011):{
                char* rm = get_reg_or_memory(bin,i,5,1);
                const char* src ,*dest;
                uint8 d_bit = GET_BIT(bin[i], 6);

                //REGISTER/MEMORY TO SEGMENT REGISTER
                if(d_bit){
                    src  =  rm;
                    dest =  SEGMENT_REGISTER[GET_SR(bin[i+1],3)];
                    printf("%s %s, %s\n", get_opcode(bin[i]), dest,src);

                //SEGMENT_REGISTER TO REGISTER/MEMORY
                }else{ 
                    dest  =  rm;
                    src =  SEGMENT_REGISTER[GET_SR(bin[i+1],3)];
                    printf("%s %s, %s\n", get_opcode(bin[i]), dest,src);
                }

                free(rm);
                i += mod_based_increment(GET_MOD(bin[i+1], 0), GET_RM(bin[i+1], 5));
            }   break;

            default:{
                //IMMEDIATE TO REGISTER
                if(GET_OPCODE(bin[i], 4) == 0b1011){
                    
                    //8-BIT IMMEDIATE
                    if(!GET_BIT(bin[i], 4)){

                        int8 immediate_value = bin[i+1];
                        const char* dest =  get_reg(bin[i], 5, GET_BIT(bin[i], 4));

                        printf("%s %s, %d\n",get_opcode(bin[i]),dest, immediate_value);

                        i = i + 2;
                    }
                    //16-BUT IMMEDIATE
                    else{

                        int16 immediate_value = (bin[i+2] << 8) | bin[i+1];
                        const char* dest =  get_reg(bin[i], 5,GET_BIT(bin[i], 4));

                        printf("%s %s, %d\n",get_opcode(bin[i]),dest, immediate_value);
                        i = i + 3;
                    }
                }
                else{
                    //printf("Currently Parsing at Index: %d\n", i);
                    printf("BINARY NOT RECOGNISED: %b %b\n", bin[i], bin[i+1]);
                    i = i + 2;
                } 
            }   break;
        }
    }
}
