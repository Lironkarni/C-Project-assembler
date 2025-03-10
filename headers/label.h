#ifndef LABEL_H
#define LABEL_H
#include "../headers/utils.h"
#include "../headers/error.h"
#include "../headers/process_input.h"

#define MAX_LABEL_LENGTH 31


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

typedef struct ext_ent_list
{
	char *label_name;
	guide_type type;
	Line *line;
	struct ext_ent_list *next;
}ext_ent_list;

extern ext_ent_list *ext_ent_list_head;

extern Symbol *symbol_table_head;

int is_valid_label(char *label, Line *line);
void add_symbol(Line *line, char *name, int instruction_index, int is_code);
Symbol *find_symbol(char *name);
void update_symbol_tabel();
int add_to_ext_ent_list(char *label_name, int type, Line *line);


#endif