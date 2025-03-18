#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "../headers/utils.h"
#include "../headers/label.h"
#include "../headers/memory_struct.h"

#include <stdint.h>

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


#endif /* FIRST_PASS_H */
