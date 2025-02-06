#include "../headers/utils.h"
#include "../headers/process_input.h"


typedef struct symbol {
    char name[MAX_LABEL_LENGTH]; 
    int address;                 
    int isData;                    
    int isCode;                    
    int isExtern;                  
    int isEntry;                  
    struct symbol *next;           
} Symbol;

extern Symbol *symbol_table_head;


int first_pass(char *file);
int process_line(char* file);
void process_word(Line* line, char* first_word);