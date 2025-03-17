#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Number of available registers in the processor */
#define NUM_OF_REG 8  

/* Size of a word/register in bits */
#define WORD_LEN 24  

/* Maximum memory size (based on 21-bit addressing) */
#define MEM_SIZE 2097151  

/* Maximum allowed length of a source file line */
#define MAX_LINE_LEN 80  

/* Minimum number of parameters required to run the program */
#define MIN_PARAM 2  

/* Initial capacity for .data directive arrays */
#define INIT_CAPACITY 10  

/* Initial memory allocation size for data structures */
#define INIT_MEMORY 100  

/* Character denoting a comment line */
#define COMMENT ';'  

/* Character indicating the end of a label */
#define COLON ':'  

/* Character used to wrap string literals */
#define QUOTE '"'  

/* Character used to separate operands */
#define COMMA ','  

/* Space character for parsing */
#define SPACE ' '  

/* Character indicating an immediate value */
#define NUMBER_SIGN '#'  

/* Character used for relative addressing (&label) */
#define AMPERSAND '&'  

/* Negative sign character */
#define MINUS '-'  

/* Positive sign character */
#define PLUS '+'  

/* Character for directives (e.g., .data, .string) */
#define POINT '.'  

/* Null character for string termination */
#define NULL_CHAR '\0'  

/* Base 10 (decimal) representation */
#define DECIMAL 10  

/* Index for external symbols */
#define EXTERN_INDEX 3  

/* Bitmask for extracting specific values */
#define MASK 0xFF  

/* Constant representing zero */
#define ZERO 0  

/* Constant representing two */
#define TWO 2  

/* Minimum allowed value for a 21-bit signed integer */
#define MIN_21BIT -1048576  

/* Maximum allowed value for a 21-bit signed integer */
#define MAX_21BIT 1048575  

/* Number of bits for shifting operations (3-bit shifts) */
#define THREE_BITS_SHIFT 3  

#endif /* GLOBALS_H */