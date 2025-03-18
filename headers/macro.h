#ifndef MACRO_H
#define MACRO_H

/* רשימת מילות מפתח של דירקטיבות באסמבלר */
static const char *directive_words[] = { "data", "string", "extern", "entry" };

/* כמות מילות מפתח של דירקטיבות באסמבלר */
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

#endif /* MACRO_H */