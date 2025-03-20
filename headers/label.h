#ifndef LABEL_H
#define LABEL_H

#include "../headers/line.h"
#include "../headers/label.h"
#include "../headers/first_pass.h"

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

/* Head pointer to the symbol table linked list */
extern Symbol *symbol_table_head;

/* Head pointer to the external labels list */
extern ext_list *ext_table_head;


/*
 * Function: is_valid_label
 * ----------------------------
 *   Validates if the given label name follows assembler naming rules.
 *   Checks length, starting character, allowed characters, and reserved words.
 *
 *   label: the label name to validate
 *   line: pointer to Line structure for error reporting
 *
 *   returns: 1 if the label is invalid (error), 0 if the label is valid
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
 * Function: update_symbol_tabel
 * ----------------------------
 *   Updates the addresses of data symbols in the symbol table after the first pass.
 */
void update_symbol_tabel();

/*
 * Function: add_to_ext_list
 * ----------------------------
 *   Adds a reference to an external label.
 *
 *   label_name: name of the external label
 *   address: address at which the external label is referenced
 *
 *   returns: 1 on successful addition, 0 otherwise
 */
int add_to_ext_list(char *label_name, int address);

/*
 * Function: free_macros
 * ----------------------------
 *   Frees allocated memory for symbols.
 */
void free_symbols();

/*
 * Function: free_macros
 * ----------------------------
 *   Frees allocated memory for externs.
 */
void free_externs();

#endif /* LABEL_H */