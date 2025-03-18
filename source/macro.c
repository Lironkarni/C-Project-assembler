#include "../headers/macro.h"
#include "../headers/globals.h"
#include "../headers/error.h"
#include "../headers/process_input.h"
#include "../headers/op_list.h"



Macro *head = NULL;  // מצביע לראש הרשימה

// הוספת מקרו לטבלה
void add_macro(const char *name, char **content, int line_count) {
    Macro *new_macro = (Macro *)malloc(sizeof(Macro));
    if (!new_macro) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    new_macro->name = strdup(name);
    new_macro->line_count = line_count;
    new_macro->next = NULL;

    new_macro->content = (char **)malloc(line_count * sizeof(char *));
    if (!new_macro->content) {
        printf("Memory allocation failed!\n");
        free(new_macro->name);
        free(new_macro);
        exit(1);
    }

    for (int i = 0; i < line_count; i++) {
        new_macro->content[i] = strdup(content[i]);
    }

    // הוספה לרשימה המקושרת (לסוף)
    if (!head) {
        head = new_macro;
    } else {
        Macro *current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_macro;
    }
}

// חיפוש מקרו לפי שם
Macro *find_macro(const char *name) {
    Macro *current = head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}


// שחרור הזיכרון של הרשימה המקושרת
void free_macros() {
    Macro *current = head;
    while (current) {
        Macro *next = current->next;

        free(current->name);
        for (int i = 0; i < current->line_count; i++) {
            free(current->content[i]);
        }
        free(current->content);
        free(current);

        current = next;
    }
    head = NULL;
}

int is_valid_macro_name(char *macro_name ,const char *filename , int line_count) {
    int valid = 0;
    //The line contains unnecessary characters.
    if (strchr(macro_name, SPACE) != NULL) {
        print_syntax_error(ERROR_CODE_15, filename ,line_count);
        valid = 1;
    }

    int operation_count = SUM_OPERATIONS;
    //check if mcro name equals to op_word
    for (int i = 0; i < operation_count; i++) {
        if (strcmp(macro_name, operation_list[i].operation_name) == 0) {
            print_syntax_error(ERROR_CODE_14, filename, line_count);
            valid = 1;
            break;  
        }
    }
    //check if mcro name equals to directive_word
    for (int i = 0; i < directive_count; i++) {
        if (strcmp(macro_name, directive_words[i]) == 0) {
            print_syntax_error(ERROR_CODE_13, filename , line_count);
            valid = 1;
        }
    }
    return valid; 
}

int is_valid_macro_end(char *line, const char *filename, int line_count) {
    char *ptr = line + 7;

    if (*ptr != NULL_CHAR && *ptr != NULL_CHAR) {
        print_syntax_error(ERROR_CODE_16, filename , line_count);
        return 1;
    }

    return 0; // אין שגיאה

}

