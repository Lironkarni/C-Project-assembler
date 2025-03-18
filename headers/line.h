#ifndef LINE_H
#define LINE_H

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

#endif