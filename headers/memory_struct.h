#ifndef MEMORY_STRUCT_H
#define MEMORY_STRUCT_H


#include "../headers/utils.h"
#include "../headers/globals.h"
#include "../headers/error.h"


#include <stdint.h>


typedef struct {
    uint8_t op_code : 6;   // 4 ביטים לצבע
    uint8_t source_address : 2;  // 4 ביטים למספר
    uint8_t source_reg : 3;   // 8 ביטים לימין
    uint8_t target_address : 2;    // 8 ביטים לשמאל
    uint8_t target_reg : 3; 
    uint8_t funct : 5; 
    uint8_t A_R_E : 3; 
} code_word;

typedef struct
{
    uint32_t data: 24; 
}data_word;

extern int DC;
extern int IC;

void add_data(data_word *data_image, int *numbers, Line *line);
void add_string_data(data_word *data_image, char *char_array, Line *line);

#endif