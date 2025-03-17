#ifndef PROCESS_INPUT_H
#define PROCESS_INPUT_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../headers/utils.h"
#include "../headers/memory_struct.h"

#define SUM_OPERATIONS 16 /** Number of supported operations in the assembler */

/**
 * @brief Enum representing different addressing methods.
 */
typedef enum address_method_type {
    IMMEDIATE,         /** Immediate addressing (e.g., #5) */
    DIRECT,            /** Direct addressing (label) */
    RELATIVE,          /** Relative addressing (&label) */
    DIRECT_REGISTER    /** Direct register addressing (e.g., r1, r2) */
} address_method_type;

/**
 * @brief Enum representing valid addressing method combinations.
 *
 * This enum defines the allowed combinations of source and destination addressing methods
 * for different operations.
 */
typedef enum valid_address_method {
    METHOD_0_1_3,  /**Valid methods: IMMEDIATE, DIRECT, DIRECT_REGISTER */
    METHOD_1,      /**Valid method: DIRECT */
    METHOD_1_3,    /**Valid methods: DIRECT, DIRECT_REGISTER */
    METHOD_1_2,    /**Valid methods: DIRECT, RELATIVE */
    NONE           /**No valid addressing methods (invalid operation) */
} valid_address_method;

/**
 * @brief Represents addressing methods for an operation.
 *
 * This struct defines the number of arguments an operation takes and the allowed
 * addressing methods for both the source and destination operands.
 */
typedef struct address_method {
    int num_args;                        /**<Number of arguments required */
    valid_address_method address_method_source; /** Allowed addressing methods for source operand */
    valid_address_method address_method_dest;   /** Allowed addressing methods for destination operand */
} address_method;

/**
 * @brief Represents an operation in the assembler.
 *
 * This struct defines an assembly operation, including its name, opcode, function code,
 * and the valid addressing methods.
 */
typedef struct op_code {
    char* operation_name; /** Name of the operation (e.g., "mov", "add") */
    int opcode;           /** The opcode value */
    int funct;            /** Function code for some operations */
    address_method address_method; /** Allowed addressing methods */
} op_code;

/**
 * @brief List of all supported operations in the assembler.
 */
extern op_code operation_list[SUM_OPERATIONS];

/**
 * @brief Checks if a given word is a valid operation.
 *
 * This function searches the list of supported operations and returns the index
 * if found.
 *
 * @param first_word The word to check.
 * @return The index of the operation if found, otherwise -1.
 */
int check_if_operation(char* first_word);

/**
 * @brief Extracts the next word from a line.
 *
 * This function retrieves the next word from a given line, skipping whitespace.
 *
 * @param line The input line.
 * @return A pointer to the extracted word.
 */
char* get_word(char* line);

/**
 * @brief Analyzes an operation and processes it accordingly.
 *
 * This function processes an operation by analyzing its operands, addressing methods,
 * and storing the relevant information in the code image.
 *
 * @param line Pointer to the `Line` structure containing the parsed line.
 * @param second_word The second word in the line (after the operation name).
 * @param is_label Flag indicating if the line starts with a label (1 if true, 0 otherwise).
 * @param first_word The first word in the line (operation name).
 * @param instruction_index The index of the instruction in the operation list.
 * @param code_image Pointer to the `code_word` structure representing the machine code.
 */
void analyse_operation(Line *line, char *second_word, int is_label, char *first_word, int instruction_index, code_word *code_image);

/**
 * @brief Determines the addressing method of an operand.
 *
 * This function identifies the addressing method used for a given operand.
 *
 * @param ptr The operand to analyze.
 * @param op_index The index of the operation in the operation list.
 * @param line Pointer to the `Line` structure containing the parsed line.
 * @return The addressing method type (IMMEDIATE, DIRECT, RELATIVE, etc.).
 */
int which_addressing_method(char *ptr, int op_index, Line *line);

/**
 * @brief Checks if a given string represents a register.
 *
 * This function determines whether the provided operand is a valid register.
 *
 * @param ptr The operand to check.
 * @return Returns 1 if the operand is a register, 0 otherwise.
 */
int is_register(char *ptr);

/**
 * @brief Validates the addressing method for an operation.
 *
 * This function checks if the provided addressing method is valid for the given
 * operation and operand position.
 *
 * @param line Pointer to the `Line` structure containing the parsed line.
 * @param method The addressing method to check.
 * @param op_index The index of the operation in the operation list.
 * @param num_args The number of arguments in the operation.
 * @return Returns 1 if the method is valid, 0 otherwise.
 */
int is_legal_method(Line *line, int method, int op_index, int num_args);

#endif /* PROCESS_INPUT_H */
