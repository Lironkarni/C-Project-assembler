#ifndef MEMORY_STRUCT_H
#define MEMORY_STRUCT_H

#include "../headers/utils.h"
#include "../headers/globals.h"
#include "../headers/error.h"

#include <stdint.h>

/**
 * @brief Represents the A, R, and E (absolute, relocatable, external) bits in machine code.
 */
typedef struct {
    int A; /**Absolute addressing bit */
    int R; /**Relocatable addressing bit */
    int E; /**External addressing bit */
} A_R_E;

/**
 * @brief Represents a code word in the machine code.
 *
 * This structure stores the bit fields that define an instruction in the assembler,
 * including opcode, addressing modes, registers, function code, and additional operands.
 */
typedef struct {
    uint8_t A_R_E : 3;        /** A, R, E bits indicating addressing type */
    uint8_t funct : 5;        /**Function code for certain operations */
    uint8_t target_reg : 3;   /**Target register */
    uint8_t target_address : 2; /**Addressing mode for the target operand */
    uint8_t source_reg : 3;   /** Source register */
    uint8_t source_address : 2; /**Addressing mode for the source operand */
    uint8_t op_code : 6;      /** Operation code */
    char *first_operand;      /** Pointer to the first operand (if any) */
    char *second_operand;     /**Pointer to the second operand (if any) */
    int place;                /** Position in memory */
    int has_label;            /** Flag indicating if a label is associated */
} code_word;

/**
 * @brief A union representing a machine code word.
 *
 * This union allows accessing the code word either as a structured `code_word`
 * or as a single 24-bit value.
 */
typedef union {
    code_word code_w;   /**< Structured representation of the code word */
    uint32_t all_bits : 24; /**< Full 24-bit machine word */
} code_union;

/**
 * @brief Represents a single data word in memory.
 *
 * This structure is used for storing data values in memory.
 */
typedef struct {
    uint32_t data : 24; /**< 24-bit data value */
} data_word;

/**
 * @brief Data Counter (DC) - Tracks the number of data words stored in memory.
 */
extern int DC;

/**
 * @brief Instruction Counter (IC) - Tracks the number of instructions stored in memory (Start from Address 100).
 */
extern int IC;

/**
 * @brief Adds data values to the data image.
 *
 * This function takes an array of numbers and stores them in the data segment of memory.
 *
 * @param data_image Pointer to the data memory structure.
 * @param numbers Array of numbers to be stored.
 * @param line Pointer to the `Line` structure containing the parsed data directive.
 */
void add_data(data_word *data_image, int *numbers, Line *line);

/**
 * @brief Adds a string to the data image.
 *
 * This function stores a string (from a `.string` directive) in memory as ASCII values.
 *
 * @param data_image Pointer to the data memory structure.
 * @param char_array The string to store in memory.
 * @param line Pointer to the `Line` structure containing the parsed string directive.
 */
void add_string_data(data_word *data_image, char *char_array, Line *line);

/**
 * @brief Adds an instruction to the code image.
 *
 * This function constructs and stores a machine code word based on the instruction's parameters.
 *
 * @param code_image Pointer to the code memory structure.
 * @param line Pointer to the `Line` structure containing the parsed instruction.
 * @param num_args Number of arguments in the instruction.
 * @param op_index Opcode index of the instruction.
 * @param funct Function code associated with the instruction.
 * @param method_src Addressing method for the source operand.
 * @param method_des Addressing method for the destination operand.
 * @param first_operand Pointer to the first operand string (if applicable).
 * @param second_operand Pointer to the second operand string (if applicable).
 */
void add_to_code_image(code_word *code_image, Line *line, int num_args, int op_index, int funct, int method_src, int method_des, char *first_operand, char *second_operand);

#endif /* MEMORY_STRUCT_H */
