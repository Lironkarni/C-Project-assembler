#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "../headers/utils.h"
#include "../headers/process_input.h"
#include "../headers/label.h"

#define INSTRUCTION_COUNT 4 /* number of instruction types */

/*
 * Function: first_pass
 * ----------------------------
 *   Executes the first pass of the assembler.
 *
 *   file: name of assembly file to process
 *
 *   returns: 0 if successful, otherwise non-zero
 */
int first_pass(char *file);

/*
 * Function: process_line
 * ----------------------------
 *   Processes a single line from the assembly file.
 *
 *   file: line content to process
 */
void process_line(char* file);

/*
 * Function: process_word
 * ----------------------------
 *   Processes the first word of a line.
 *
 *   line: pointer to Line structure
 *   first_word: first word extracted from line
 */
void process_word(Line* line, char* first_word);

/*
 * Function: which_instruction
 * ----------------------------
 *   Determines instruction type by its name.
 *
 *   word: instruction word to check
 *
 *   returns: index of instruction, or -1 if invalid
 */
int which_instruction(char *word);

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
 * Function: test
 * ----------------------------
 *   Debugging function to print IC and DC counters.
 *
 *   dc: current data counter
 *   ic: current instruction counter
 */
void test(int dc, int ic);

/*
 * Function: print_bits
 * ----------------------------
 *   Prints value as binary representation.
 *
 *   value: value to print
 *   bits: number of bits to display
 */
void print_bits(uint32_t value, int bits);

#endif /* FIRST_PASS_H */
