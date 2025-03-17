#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "../headers/utils.h"
#include "../headers/process_input.h"
#include "../headers/label.h"

#define INSTRUCTION_COUNT 4 /* Number of instruction types */

/**
 * @brief Executes the first pass of the assembler.
 *
 * This function reads the am file and processes each line, handling
 * labels, instructions, and directives. It collects relevant information for the second pass.
 *
 * @param file The name of the assembly file to process.
 * @return Returns 0 if no errors were encountered, otherwise returns a non-zero value.
 */
int first_pass(char *file);

/**
 * @brief Processes a single line from the am file.
 *
 * This function takes a line from the am file and determines its type (instruction, directive, label, etc.).
 * It extracts the first word and processes it accordingly.
 *
 * @param file The line from the am file to process.
 */
void process_line(char* file);

/**
 * @brief Processes the first word in a line.
 *
 * This function analyzes the first word in a line and determines how it should be processed.
 * It checks if the word is an instruction, directive, or label.
 *
 * @param line A pointer to the `Line` structure containing the parsed information.
 * @param first_word The first word extracted from the line.
 */
void process_word(Line* line, char* first_word);

/**
 * @brief Determines which instruction is used.
 *
 * This function checks if the given word matches a known instruction and returns its index.
 * If the word is not a valid instruction, it returns -1.
 *
 * @param word The word to check.
 * @return The instruction index if found, otherwise -1.
 */
int which_instruction(char *word);

/**
 * @brief Extracts data values from a .data directive.
 *
 * This function parses a `.data` directive and extracts the numerical values into an array.
 *
 * @param line A pointer to the `Line` structure containing the parsed instruction.
 * @param inst_index The index of the `.data` directive in the instruction set.
 * @param numbers A pointer to an array where the extracted numbers will be stored.
 * @return The number of values extracted, or -1 if an error occurred.
 */
int get_data(Line *line, int inst_index, int **numbers);

/**
 * @brief Extracts a string from a .string directive.
 *
 * This function extracts the string value from a `.string` directive and stores it.
 *
 * @param line A pointer to the `Line` structure containing the parsed instruction.
 * @param inst_index The index of the `.string` directive in the instruction set.
 * @param string_value A pointer to a char array where the extracted string will be stored.
 * @return Returns 1 if successful, 0 otherwise.
 */
int get_string_data(Line *line, int inst_index, char **string_value);

/**
 * @brief Debugging function for first pass.
 *
 * This function prints the instruction counter (IC) and data counter (DC) for debugging purposes.
 *
 * @param dc The current data counter.
 * @param ic The current instruction counter.
 */
void test(int dc, int ic);

/**
 * @brief Prints a value in binary representation.
 *
 * This function prints a given value as a binary string with the specified number of bits.
 *
 * @param value The value to print.
 * @param bits The number of bits to display.
 */
void print_bits(uint32_t value, int bits);

#endif /* FIRST_PASS_H */
