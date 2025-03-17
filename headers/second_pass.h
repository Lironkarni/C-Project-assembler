#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "../headers/label.h"

/**
 * @brief Executes the second pass of the assembler.
 *
 * This function processes the assembly file in a second pass, resolving symbol references,
 * updating addresses, and preparing the final output files (ext, ent and ob).
 *
 * @param file The name of the assembly file being processed.
 * @param head Pointer to the head of the external label list.
 * @param symbol_table_head Pointer to the head of the symbol table.
 * @param code_image Pointer to the code image where machine instructions are stored.
 * @param data_image Pointer to the data image where `.data` and `.string` values are stored.
 */
void second_pass(char *file, ext_list *head, Symbol *symbol_table_head, code_word *code_image, data_word *data_image);

/**
 * @brief Checks if an operand is an external label and processes it accordingly.
 *
 * This function determines if a given operand is an external symbol and updates
 * the external label list if necessary.
 *
 * @param ext_list_head Pointer to the head of the external labels list.
 * @param operand_name The name of the operand being checked.
 * @param line Pointer to the `Line` structure containing the parsed instruction.
 */
void check_externs(ext_list *ext_list_head, char operand_name, Line *line);

/**
 * @brief Generates the `.ent` file containing entry symbols.
 *
 * This function creates an `.ent` (entry) file that lists all symbols declared
 * as `.entry`, along with their memory addresses.
 *
 * @param filename The name of the output file (without extension).
 * @param symbol_table_head Pointer to the head of the symbol table.
 */
void make_ent_file(const char *filename, Symbol *symbol_table_head);

/**
 * @brief Generates the `.ob` file containing number of Data and Code lines and The machine Hex code.
 *
 * This function creates an `.ob` (object) file that contains the translated
 * machine code from the assembler, including both code and data segments.
 *
 * @param filename The name of the output file (without extension).
 * @param code_image Pointer to the code image where machine instructions are stored.
 * @param ic The final instruction counter value.
 * @param data_image Pointer to the data image where `.data` and `.string` values are stored.
 * @param dc The final data counter value.
 */
void make_ob_file(const char *filename, code_word *code_image, int ic, data_word *data_image, int dc);

#endif /* SECOND_PASS_H */
