#ifndef MEMORY_STRUCT_H
#define MEMORY_STRUCT_H

#include "../headers/utils.h"
#include "../headers/globals.h"
#include "../headers/error.h"

#include <stdint.h>

typedef struct
{
    int A;
    int R;
    int E;
} A_R_E;

typedef struct
{
    uint8_t A_R_E : 3;
    uint8_t funct : 5;
    uint8_t target_reg : 3;
    uint8_t target_address : 2;
    uint8_t source_reg : 3;
    uint8_t source_address : 2;
    uint8_t op_code : 6;
} code_word;

typedef union
{
    code_word code_w;
    uint32_t all_bits : 24;
} code_union;

typedef struct
{
    uint32_t data : 24;
} data_word;

extern int DC;
extern int IC;

void add_data(data_word *data_image, int *numbers, Line *line);
void add_string_data(data_word *data_image, char *char_array, Line *line);
void add_to_code_image(code_word *code_image, Line *line, int num_args, int op_index, int funct, int method_src, int method_des, char *first_operand, char *second_operand);

#endif