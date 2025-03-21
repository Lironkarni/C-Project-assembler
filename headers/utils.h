#ifndef FILE_COPY_H
#define FILE_COPY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/*
 * Function: remove_extra_spaces
 * ----------------------------
 *   Removes extra spaces from a line.
 *
 *   line: line to clean
 */
void remove_extra_spaces(char *line);

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
 * Function: extraneous_text
 * ----------------------------
 *   Checks if extraneous text exists after command.
 *
 *   command: command string
 *
 *   returns: 1 if extraneous text found, 0 otherwise
 */
int extraneous_text(char *command);


/*
 * Function: get_word
 * ----------------------------
 *   Extracts the next word from the line.
 *
 *   line: input line
 *
 *   returns: pointer to extracted word
 */
char* get_word(char* line);

char *copy_name(const char *name);

#endif /* FILE_COPY_H */
