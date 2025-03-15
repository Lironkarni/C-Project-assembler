#ifndef LABEL_H
#define LABEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../headers/line.h"
#include "../headers/error.h"
#include "../headers/globals.h"
#include "../headers/memory_struct.h"
#include "../headers/macros.h"



typedef enum guide_type
{
	DATA,
	STRING,
	ENTRY,
	EXTERNAL,
	OPERATION
}guide_type;

typedef struct symbol {
    char name[MAX_LABEL_LENGTH]; 
    int address;                 
    guide_type type;                
    struct symbol *next;           
} Symbol;


extern Symbol *symbol_table_head;
extern char *instruction_list[INSTRUCTION_COUNT];

int is_valid_label(char *label, Line *line);
void add_symbol(Line *line, char *name, int instruction_index, int is_code);
Symbol *find_symbol(char *name);
void update_symbol_tabel();

#endif