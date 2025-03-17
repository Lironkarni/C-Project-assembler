#ifndef PROCESS_INPUT_H
#define PROCESS_INPUT_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../headers/utils.h"
#include "../headers/memory_struct.h"

#define SUM_OPERATIONS 16 /* number of supported assembler operations */

/*
 * Enum: address_method_type
 * ----------------------------
 *   Defines addressing method types.
 */
typedef enum address_method_type {
    IMMEDIATE,        /* immediate addressing (#5) */
    DIRECT,           /* direct addressing (label) */
    RELATIVE,         /* relative addressing (&label) */
    DIRECT_REGISTER   /* direct register addressing (r1, r2) */
} address_method_type;

/*
 * Enum: valid_address_method
 * ----------------------------
 *   Defines valid addressing method combinations.
 */
typedef enum valid_address_method {
    METHOD_0_1_3, /* valid: IMMEDIATE, DIRECT, DIRECT_REGISTER */
    METHOD_1,     /* valid: DIRECT */
    METHOD_1_3,   /* valid: DIRECT, DIRECT_REGISTER */
    METHOD_1_2,   /* valid: DIRECT, RELATIVE */
    NONE          /* no valid methods (invalid operation) */
} valid_address_method;

/*
 * Struct: address_method
 * ----------------------------
 *   Defines addressing methods allowed for operations.
 *
 *   num_args: number of operands required
 *   address_method_source: allowed methods for source operand
 *   address_method_dest: allowed methods for destination operand
 */
typedef struct address_method {
    int num_args; 
    valid_address_method address_method_source; 
    valid_address_method address_method_dest;   
} address_method;

/*
 * Struct: op_code
 * ----------------------------
 *   Represents an assembler operation.
 *
 *   operation_name: operation name ("mov", "add", etc.)
 *   opcode: numeric opcode
 *   funct: numeric function code (if applicable)
 *   address_method: valid addressing methods
 */
typedef struct op_code {
    char* operation_name; 
    int opcode;           
    int funct;            
    address_method address_method; 
} op_code;

/* List of supported assembler operations */
extern op_code operation_list[SUM_OPERATIONS];

/*
 * Function: check_if_operation
 * ----------------------------
 *   Checks if word is a valid operation.
 *
 *   first_word: word to check
 *
 *   returns: operation index or -1 if not found
 */
int check_if_operation(char* first_word);

/*
 * Function: get_word
 * ----------------------------
 *   Extracts the next word from the line.
 *
 *   line: input line
 *
 *   returns: pointer to extracted word
 */
char* get_word(char* line);

/*
 * Function: analyse_operation
 * ----------------------------
 *   Processes operation details.
 *
 *   line: pointer to parsed line info
 *   second_word: second word after operation
 *   is_label: flag if line has label (1: yes, 0: no)
 *   first_word: operation name
 *   instruction_index: index in operation list
 *   code_image: pointer to code memory structure
 */
void analyse_operation(Line *line, char *second_word, int is_label, char *first_word, int instruction_index, code_word *code_image);

/*
 * Function: which_addressing_method
 * ----------------------------
 *   Determines operand addressing method.
 *
 *   ptr: operand to analyze
 *   op_index: operation index
 *   line: pointer to parsed line info
 *
 *   returns: addressing method type
 */
int which_addressing_method(char *ptr, int op_index, Line *line);

/*
 * Function: is_register
 * ----------------------------
 *   Checks if operand is a register.
 *
 *   ptr: operand to check
 *
 *   returns: 1 if register, 0 otherwise
 */
int is_register(char *ptr);

/*
 * Function: is_legal_method
 * ----------------------------
 *   Checks validity of addressing method for operation.
 *
 *   line: pointer to parsed line info
 *   method: addressing method to validate
 *   op_index: operation index
 *   num_args: number of operands
 *
 *   returns: 1 if valid method, 0 otherwise
 */
int is_legal_method(Line *line, int method, int op_index, int num_args);

#endif /* PROCESS_INPUT_H */
