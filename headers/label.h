#ifndef LABEL_H
#define LABEL_H

#include "../headers/utils.h"
#include "../headers/process_input.h"
#include "../headers/label.h"

#define MAX_LABEL_LENGTH 31 /* maximum length of a label name */

/*
 * Enum: guide_type
 * ----------------------------
 *   Represents types of assembler directives.
 */
typedef enum guide_type {
    DATA,       /* .data directive */
    STRING,     /* .string directive */
    ENTRY,      /* .entry directive */
    EXTERNAL,   /* .extern directive */
    OPERATION   /* assembler operation (command) */
} guide_type;

/*
 * Struct: Symbol
 * ----------------------------
 *   Represents a label in the symbol table.
 *
 *   name: label name
 *   address: memory address of the label
 *   type: type of label
 *   next: pointer to next symbol in the list
 */
typedef struct symbol {
    char name[MAX_LABEL_LENGTH];
    int address;
    guide_type type;
    struct symbol *next;
} Symbol;

/*
 * Struct: ext_list
 * ----------------------------
 *   Represents an external label reference.
 *
 *   label_name: external label name
 *   address: reference address
 *   next: pointer to next external label
 */
typedef struct ext_list {
    char *label_name;
    int address;
    struct ext_list *next;
} ext_list;

extern Symbol *symbol_table_head; /* head pointer for symbol table */

/*
 * Function: is_valid_label
 * ----------------------------
 *   Checks if a label name is valid.
 *
 *   label: label name to validate
 *   line: pointer to Line structure
 *
 *   returns: 1 if valid, 0 otherwise
 */
int is_valid_label(char *label, Line *line);

/*
 * Function: add_symbol
 * ----------------------------
 *   Adds a new symbol to the symbol table.
 *
 *   line: pointer to Line structure
 *   name: label name
 *   instruction_index: associated instruction index
 *   is_code: 1 if code symbol, 0 if data symbol
 */
void add_symbol(Line *line, char *name, int instruction_index, int is_code);

/*
 * Function: find_symbol
 * ----------------------------
 *   Finds a symbol in the symbol table.
 *
 *   name: label name to find
 *
 *   returns: pointer to Symbol if found, otherwise NULL
 */
Symbol *find_symbol(char *name);

/*
 * Function: update_symbol_table
 * ----------------------------
 *   Updates symbol addresses after first pass.
 */
void update_symbol_table();

/*
 * Function: add_to_ext_list
 * ----------------------------
 *   Adds an external label reference.
 *
 *   ext_list_head: head pointer for external label list
 *   label_name: external label name
 *   address: address where the external label is used
 *
 *   returns: 1 if successful, 0 otherwise
 */
int add_to_ext_list(ext_list *ext_list_head, char *label_name, int address);

#endif /* LABEL_H */