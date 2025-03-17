#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Represents an error with an associated error code and error message.
 *
 * This struct is used to store error information, including a unique identifier
 * and user-readable message describing the error.
 */
typedef struct error {
    int err_code;     /* identifier for the error */
    char* err_message; /* Error message associated with the error */
} error;

/**
 * @brief Enumeration of possible error codes.
 *
 * This enum defines a set of error codes used throughout the assembler to 
 * categorize and report different types of errors.
 */
typedef enum error_codes {
    ERROR_CODE_0 = 0,  /* No error found */
    ERROR_CODE_1,      /* No files entered, can't run program */
    ERROR_CODE_2,      /* File does not exist */
    ERROR_CODE_3,      /* Failed to allocate memory */
    ERROR_CODE_4,      /* Failed to open file */
    ERROR_CODE_5,      /* Unable to open file for reading */
    ERROR_CODE_6,      /* Unable to open file for writing */
    ERROR_CODE_7,      /* Error deleting file */
    ERROR_CODE_8,      /* Line is too long, exceeds allowed characters */
    ERROR_CODE_9,      /* Invalid label name: cannot be an assembly instruction */
    ERROR_CODE_10,     /* Invalid label name: must start with an alphabetic character */
    ERROR_CODE_11,     /* Invalid label name: exceeds 31 characters */
    ERROR_CODE_12,     /* Invalid label name: should contain only letters and digits */
    ERROR_CODE_13,     /* Invalid macro name: cannot be a directive name */
    ERROR_CODE_14,     /* Invalid macro name: cannot be an instruction name */
    ERROR_CODE_15,     /* Extra characters found in macro open line */
    ERROR_CODE_16,     /* Extra characters found in macro close line */
    ERROR_CODE_17,     /* Invalid label name: cannot be the same as a macro name */
    ERROR_CODE_18,     /* Label name already exists */
    ERROR_CODE_19,     /* Label name cannot be a register name */
    ERROR_CODE_20,     /* String instruction is not enclosed in quotes */
    ERROR_CODE_21,     /* Extraneous text after end of command */
    ERROR_CODE_22,     /* Expected a number in a data instruction */
    ERROR_CODE_23,     /* Missing comma between numbers in data instruction */
    ERROR_CODE_24,     /* Data instruction cannot end with a comma */
    ERROR_CODE_25,     /* Unrecognized command, check syntax */
    ERROR_CODE_26,     /* This operation is missing an operand */
    ERROR_CODE_27,     /* Missing comma between operands */
    ERROR_CODE_28,     /* Illegal comma between operation name and operand */
    ERROR_CODE_29,     /* Illegal comma | Expected only one operand */
    ERROR_CODE_30,     /* This operation is missing operands */
    ERROR_CODE_31,     /* Expected numeric characters in direct addressing */
    ERROR_CODE_32,     /* Invalid use of '&' for operations other than JMP/BNE/JSR */
    ERROR_CODE_33,     /* Invalid command | Expected a comma between operands */
    ERROR_CODE_34,     /* Invalid addressing method for destination operand */
    ERROR_CODE_35,     /* Invalid comma | Expected only one comma */
    ERROR_CODE_36,     /* Invalid operand | Value out of range for 'immediate' method (21 bits) */
    ERROR_CODE_37,     /* IC limit reached, cannot add to memory */
    ERROR_CODE_38,     /* Label declared as both a label and an extern in the same file */
    ERROR_CODE_39,     /* Label declared as entry but not as a label in the same file */
    ERROR_CODE_40,     /* Label used as an operand but not declared as a label */
    ERROR_CODE_41,     /* Numbers in data instruction must be integers (not decimal) */
    ERROR_CODE_42      /* Operand of type number (#) must be an integer (not decimal) */
} error_codes;

/**
 * @brief Flags that indicate if errors were found during different assembly phases.
 *
 * These external variables track whether errors occurred during:
 * - The pre-assembler phase
 * - The first pass
 * - The second pass
 */
extern int FOUND_ERROR_IN_PRE_ASSEMBLER; /**< Flag for pre-assembler errors */
extern int FOUND_ERROR_IN_FIRST_PASS;    /**< Flag for first pass errors */
extern int FOUND_ERROR_IN_SECOND_PASS;   /**< Flag for second pass errors */

/**
 * @brief Prints a system error message.
 *
 * This function prints an error message associated with a system-related error.
 *
 * @param error_code The error code representing the error.
 */
void print_system_error(int error_code);

/**
 * @brief Prints a syntax error message with file name and line number.
 *
 * This function reports syntax errors encountered during assembly processing.
 * It also sets the `FOUND_ERROR_IN_PRE_ASSEMBLER` flag if the error is related
 * to macros or extra characters in macro declarations.
 *
 * @param error_code The error code representing the encountered error.
 * @param file_name The name of the file where the error was found.
 * @param line_number The line number where the error occurred.
 */
void print_syntax_error(int error_code, const char* file_name, int line_number);

#endif /* ERROR_H */
