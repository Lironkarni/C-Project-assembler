#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include "../headers/utils.h"
#include "../headers/process_input.h"
#define INSTRUCTION_COUNT 4

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

int first_pass(char *file);
int process_line(char* file);
void process_word(Line* line, char* first_word);
int which_instruction(char *word);
void add_symbol(Line *line, char *name, int instruction_index, int is_code);
Symbol *find_symbol(char *name);
int get_data(Line *line, int inst_index, int **numbers);
int get_string_data( Line *line, int inst_index, char **string_value);

void test(int dc);

#endif