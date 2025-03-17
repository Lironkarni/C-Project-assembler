#ifndef LABEL_H
#define LABEL_H

#include "../headers/utils.h"
#include "../headers/error.h"
#include "../headers/process_input.h"

#define MAX_LABEL_LENGTH 31 /**< Maximum allowed length for a label name */

/**
 * @brief Enum representing different types of assembler directives.
 */
typedef enum guide_type {
    DATA,      /** .data directive */
    STRING,    /** .string directive */
    ENTRY,     /** .entry directive */
    EXTERNAL,  /** .extern directive */
    OPERATION  /** operation (command) */
} guide_type;

/**
 * @brief Represents a symbol (label) in the assembler's symbol table.
 *
 * This struct stores information about labels, including their name, memory address,
 * type (data, string, entry, external, or operation), and a pointer to the next symbol.
 */
typedef struct symbol {
    char name[MAX_LABEL_LENGTH]; /** The name of the label */
    int address;                 /** Memory address assigned to the label */
    guide_type type;             /** Type of the label (data, string, entry, etc.) */
    struct symbol *next;         /** Pointer to the next symbol in the list */
} Symbol;

/**
 * @brief Represents an entry in the list of external labels.
 *
 * This struct stores information about labels defined as `.extern`, including
 * the label name, its memory address, and a pointer to the next external label.
 */
typedef struct ext_list {
    char *label_name;       /** Name of the external label */
    int address;            /** Address where the external label is used */
    struct ext_list *next;  /** Pointer to the next external label in the list */
} ext_list;

/**
 * @brief Head of the symbol table linked list.
 *
 * This variable points to the first symbol in the symbol table.
 */
extern Symbol *symbol_table_head;

/**
 * @brief Checks if a given label name is valid.
 *
 * This function verifies that a label follows assembler naming rules.
 * A valid label:
 * - Starts with a letter.
 * - Contains only alphanumeric characters.
 * - Does not exceed the maximum label length.
 * - Is not a reserved keyword.
 *
 * @param label The label name to check.
 * @param line Pointer to the `Line` structure containing the parsed line.
 * @return Returns 1 if the label is valid, 0 otherwise.
 */
int is_valid_label(char *label, Line *line);

/**
 * @brief Adds a new symbol to the symbol table.
 *
 * This function creates a new symbol entry and adds it to the symbol table.
 * The symbol can be a data label, a code label, or a directive.
 *
 * @param line Pointer to the `Line` structure containing the parsed line.
 * @param name The name of the label to add.
 * @param instruction_index The instruction index associated with this symbol.
 * @param is_code A flag indicating whether the symbol is for code (1) or data (0).
 */
void add_symbol(Line *line, char *name, int instruction_index, int is_code);

/**
 * @brief Finds a symbol in the symbol table.
 *
 * This function searches for a label by name in the symbol table.
 *
 * @param name The label name to search for.
 * @return A pointer to the `Symbol` structure if found, otherwise NULL.
 */
Symbol *find_symbol(char *name);

/**
 * @brief Updates the symbol table after the first pass.
 *
 * This function processes symbols and updates their addresses based on the
 * instruction and data counters after the first pass.
 */
void update_symbol_tabel();

/**
 * @brief Adds a label to the external label list.
 *
 * This function adds a reference to an external label in the ext_list.
 *
 * @param ext_list_head Pointer to the head of the external labels list.
 * @param label_name The name of the external label.
 * @param address The address where the label is referenced.
 * @return Returns 1 if successful, 0 otherwise.
 */
int add_to_ext_list(ext_list *ext_list_head, char *label_name, int address);

#endif /* LABEL_H */
