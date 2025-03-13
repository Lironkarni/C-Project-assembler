#ifndef LINE_H
#define LINE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/error.h"


typedef struct Line
{
    // FILE *file;
    char *file_name;
    char *data;
    int line_number;
}Line;



Line *create_line(char* temp_line,char* file,int line_number);

#endif