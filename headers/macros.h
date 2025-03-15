#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/globals.h"
#include "../headers/error.h"
#include "../headers/op_list.h"
#include "../headers/first_pass.h"



typedef struct Macro {
    char *name;             // שם המקרו בזיכרון דינאמי
    char **content;         // מערך דינאמי של שורות המקרו
    int line_count;
    struct Macro *next;     // מצביע למקרו הבא ברשימה המקושרת
} Macro;

void add_macro(const char *name, char **content, int line_count);

Macro *find_macro(const char *name);

void free_macros();

int is_valid_macro_name(char *macro_name ,const char *filename , int line_count);

int is_valid_macro_end(char *line, const char *filename, int line_count);

#endif