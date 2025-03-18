#ifndef MEMORY_STRUCT_H
#define MEMORY_STRUCT_H

#include "../headers/globals.h"
#include "../headers/line.h"


#include <stdint.h>

/*
 * Struct: A_R_E
 * ----------------------------
 *   Represents the Absolute, Relocatable, and External addressing bits.
 *
 *   A: absolute addressing bit
 *   R: relocatable addressing bit
 *   E: external addressing bit
 */
typedef struct {
    int A; 
    int R; 
    int E; 
} A_R_E;

/*
 * Struct: code_word
 * ----------------------------
 *   Represents a machine instruction word.
 *
 *   A_R_E: addressing type bits
 *   funct: function code
 *   target_reg: target register number
 *   target_address: addressing mode for target operand
 *   source_reg: source register number
 *   source_address: addressing mode for source operand
 *   op_code: operation code
 *   first_operand: first operand (if exists)
 *   second_operand: second operand (if exists)
 *   place: memory location of the code word
 *   has_label: flag indicating associated label presence
 */
typedef struct {
    uint8_t A_R_E : 3;      
    uint8_t funct : 5;        
    uint8_t target_reg : 3;   
    uint8_t target_address : 2;
    uint8_t source_reg : 3;  
    uint8_t source_address : 2; 
    uint8_t op_code : 6;     
    char *first_operand;     
    char *second_operand;     
    int place;              
    int has_label;           
} code_word;

/*
 * Union: code_union
 * ----------------------------
 *   Represents a code word either as structured data or as a 24-bit value.
 *
 *   code_w: structured representation of instruction
 *   all_bits: full 24-bit machine word
 */
typedef union {
    code_word code_w;    
    uint32_t all_bits : 24; 
} code_union;

/*
 * Struct: data_word
 * ----------------------------
 *   Represents a data word stored in memory.
 *
 *   data: 24-bit numeric value
 */
typedef struct {
    uint32_t data : 24;
} data_word;

extern int DC; /* data counter */
extern int IC; /* instruction counter (starts at 100) */

/*
 * Function: get_data
 * ----------------------------
 *   Extracts numeric data from a .data directive.
 *
 *   line: pointer to Line structure
 *   inst_index: directive index
 *   numbers: array to store extracted numbers
 *
 *   returns: number of values extracted, or -1 on error
 */
int get_data(Line *line, int inst_index, int **numbers);


/*
 * Function: get_string_data
 * ----------------------------
 *   Extracts string data from a .string directive.
 *
 *   line: pointer to Line structure
 *   inst_index: directive index
 *   string_value: extracted string
 *
 *   returns: 1 if successful, otherwise 0
 */
int get_string_data(Line *line, int inst_index, char **string_value);

/*
 * Function: add_data
 * ----------------------------
 *   Adds numeric values to data segment memory.
 *
 *   data_image: pointer to data memory array
 *   numbers: array of numbers to store
 *   line: pointer to line information
 */
void add_data(data_word *data_image, int *numbers, Line *line);

/*
 * Function: add_string_data
 * ----------------------------
 *   Adds a string as ASCII values to the data segment.
 *
 *   data_image: pointer to data memory array
 *   char_array: string to store
 *   line: pointer to line information
 */
void add_string_data(data_word *data_image, char *char_array, Line *line);

/*
 * Function: add_to_code_image
 * ----------------------------
 *   Adds an instruction to the code segment memory.
 *
 *   code_image: pointer to code memory array
 *   line: pointer to line information
 *   num_args: number of operands
 *   op_index: opcode index
 *   funct: function code
 *   method_src: source operand addressing method
 *   method_des: destination operand addressing method
 *   first_operand: first operand (if applicable)
 *   second_operand: second operand (if applicable)
 */
void add_to_code_image(code_word *code_image, Line *line, int num_args, int op_index, int funct, int method_src, int method_des, char *first_operand, char *second_operand);

#endif /* MEMORY_STRUCT_H */
