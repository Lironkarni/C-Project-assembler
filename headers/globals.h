#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Number of available registers in the Processor.
 */
#define NUM_OF_REG 8  

/**
 * @brief The size of a word/register in bits.
 */
#define WORD_LEN 24  

/**
 * @brief Maximum memory size, based on 21-bit addressing (2^21 - 1).
 */
#define MEM_SIZE 2097151  

/**
 * @brief Maximum allowed length of a line in the source file.
 */
#define MAX_LINE_LEN 80  

/**
 * @brief Minimum number of parameters required for the program to start.
 */
#define MIN_PARAM 2  

/**
 * @brief Initial capacity for arrays storing numbers in .data directives.
 */
#define INIT_CAPACITY 10  

/**
 * @brief Initial memory allocation size for data structures.
 */
#define INIT_MEMORY 100  

/**
 * @brief Character that denotes a comment in the source file.
 */
#define COMMENT ';'  

/**
 * @brief Character used to indicate the end of a label.
 */
#define COLON ':'  

/**
 * @brief Character used to wrap string literals.
 */
#define QUOTE '"'  

/**
 * @brief Character used to separate operands in instructions.
 */
#define COMMA ','  

/**
 * @brief The space character, often used for parsing.
 */
#define SPACE ' '  

/**
 * @brief Character indicating an immediate value in instructions.
 */
#define NUMBER_SIGN '#'  

/**
 * @brief Character used for addressing methods involving labels.
 */
#define AMPERSAND '&'  

/**
 * @brief Character representing a negative sign.
 */
#define MINUS '-'  

/**
 * @brief Character representing a positive sign.
 */
#define PLUS '+'  

/**
 * @brief Character used for directives (e.g., .data, .string).
 */
#define POINT '.'  

/**
 * @brief Null character, commonly used for string termination.
 */
#define NULL_CHAR '\0'  

/**
 * @brief Base 10 (decimal) representation.
 */
#define DECIMAL 10  

/**
 * @brief Index for external symbols in certain data structures.
 */
#define EXTERN_INDEX 3  

/**
 * @brief Bitmask used for extracting specific values.
 */
#define MASK 0xFF  

/**
 * @brief Constant representing the value zero.
 */
#define ZERO 0  

/**
 * @brief Constant representing the value two.
 */
#define TWO 2  

/**
 * @brief Minimum value allowed for a 21-bit signed integer.
 */
#define MIN_21BIT -1048576  

/**
 * @brief Maximum value allowed for a 21-bit signed integer.
 */
#define MAX_21BIT 1048575  

/**
 * @brief Shift value for operations involving 3-bit manipulation.
 */
#define THREE_BITS_SHIFT 3  

#endif /* GLOBALS_H */
