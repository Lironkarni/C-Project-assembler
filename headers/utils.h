#ifndef FILE_COPY_H
#define FILE_COPY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024 /* maximum line length in source file */

/* List of directive keywords in the assembler */
static const char *directive_words[] = { "data", "string", "extern", "entry" };

/* Number of assembler directive keywords */
static const int directive_count = sizeof(directive_words) / sizeof(directive_words[0]);

/*
 * Struct: Macro
 * ----------------------------
 *   Represents a macro definition.
 *
 *   name: macro name
 *   content: lines of macro content
 *   line_count: number of lines in macro
 *   next: pointer to next macro
 */
typedef struct Macro {
    char *name;        /* macro name */
    char **content;    /* macro content lines */
    int line_count;    /* number of lines */
    struct Macro *next;/* pointer to next macro */
} Macro;

/*
 * Struct: Line
 * ----------------------------
 *   Represents a line from the source file.
 *
 *   file_name: source file name
 *   data: line content
 *   line_number: line number in source file
 */
typedef struct Line {
    char *file_name; /* source file name */
    char *data;      /* line content */
    int line_number; /* line number */
} Line;

/*
 * Function: add_macro
 * ----------------------------
 *   Adds a new macro to the macro list.
 *
 *   name: macro name
 *   content: array of macro lines
 *   line_count: number of lines
 */
void add_macro(const char *name, char **content, int line_count);

/*
 * Function: find_macro
 * ----------------------------
 *   Finds a macro by name.
 *
 *   name: macro name
 *
 *   returns: pointer to Macro if found, NULL otherwise
 */
Macro *find_macro(const char *name);

/*
 * Function: remove_extra_spaces
 * ----------------------------
 *   Removes extra spaces from a line.
 *
 *   line: line to clean
 */
void remove_extra_spaces(char *line);

/*
 * Function: pre_assembler
 * ----------------------------
 *   Performs pre-assembly, handling macro expansions.
 *
 *   filename: source file name
 */
void pre_assembler(const char *filename);

/*
 * Function: load_file
 * ----------------------------
 *   Loads file content into memory.
 *
 *   filename: file name
 *
 *   returns: pointer to loaded content
 */
char *load_file(char *filename);

/*
 * Function: is_empty_line
 * ----------------------------
 *   Checks if line is empty or whitespace only.
 *
 *   line: line to check
 *
 *   returns: 1 if empty, 0 otherwise
 */
int is_empty_line(const char *line);

/*
 * Function: free_macros
 * ----------------------------
 *   Frees allocated memory for macros.
 */
void free_macros();

/*
 * Function: is_valid_macro_name
 * ----------------------------
 *   Checks validity of macro name.
 *
 *   macro_name: macro name to validate
 *   filename: source file name
 *   line: line number in source file
 *
 *   returns: 1 if valid, 0 otherwise
 */
int is_valid_macro_name(char *macro_name, const char *filename, int line);

/*
 * Function: is_valid_macro_end
 * ----------------------------
 *   Checks if macro end definition ('endmacro') is correct.
 *
 *   line: current line content
 *   filename: source file name
 *   line_count: current line number
 *
 *   returns: 1 if valid, 0 otherwise
 */
int is_valid_macro_end(char *line, const char *filename, int line_count);

/*
 * Function: delete_am_file
 * ----------------------------
 *   Deletes pre-assembled (.am) file.
 *
 *   filename: base file name (without extension)
 */
void delete_am_file(const char *filename);

/*
 * Function: create_line
 * ----------------------------
 *   Creates a new Line structure.
 *
 *   temp_line: line content
 *   file: source file name
 *   line_number: line number
 *
 *   returns: pointer to new Line
 */
Line *create_line(char* temp_line, char* file, int line_number);

/*
 * Function: extraneous_text
 * ----------------------------
 *   Checks if extraneous text exists after command.
 *
 *   command: command string
 *
 *   returns: 1 if extraneous text found, 0 otherwise
 */
int extraneous_text(char *command);

#endif /* FILE_COPY_H */
