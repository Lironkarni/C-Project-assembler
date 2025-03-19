#include "../headers/macro.h"
#include "../headers/globals.h"
#include "../headers/error.h"
#include "../headers/op_list.h"
#include "../headers/utils.h"


Macro *head = NULL;  // pointer to the head of the macro list

void add_macro(const char *name, char **content, int line_count) {
    /* Allocate memory for the new macro node */
    Macro *new_macro = (Macro *)malloc(sizeof(Macro));
    if (!new_macro)
    {
        print_system_error(ERROR_CODE_3);
        exit(1);
    }
        /* Store the macro name */
    new_macro->name = copy_name(name);
    if (!new_macro->name)
    {
        free(new_macro);
        print_system_error(ERROR_CODE_3);
        exit(1);
    }
    new_macro->line_count = line_count;
    new_macro->next = NULL;

    /* Allocate memory for the macro content lines */
    new_macro->content = (char **)malloc(line_count * sizeof(char *));
    if (!new_macro->content)
    {
        print_system_error(ERROR_CODE_3);
        free(new_macro->name);
        free(new_macro);
        exit(1);
    }

    /* Copy each line of the macro content */
    for (int i = 0; i < line_count; i++)
    {
        new_macro->content[i] = copy_name(content[i]);
        if (!new_macro->content[i])
        { // allocate failed
            for (int j = 0; j < i; j++)
            {
                free(new_macro->content[j]);
            }
            free(new_macro->content);
            free(new_macro->name);
            free(new_macro);
            print_system_error(ERROR_CODE_3);
            exit(1);
        }
    }

    /* Insert the new macro into the linked list */
    if (!head) {
        head = new_macro; // If the list is empty, set as head
    } else {
        Macro *current = head;

        /* Traverse to the end of the list */
        while (current->next) {
            current = current->next;
        }
        current->next = new_macro; // Append the new macro
    }
}

Macro *find_macro(const char *name) {
    Macro *current = head;
    /* Traverse the macro list */
    while (current) {
        /* Compare the current macro's name with the target name */
        if (strcmp(current->name, name) == 0) {
            return current;  // Macro found
        }
        current = current->next;
    }
    return NULL; // Macro not found
}

void free_macros() {
    Macro *current = head;
    while (current) {
        Macro *next = current->next;  // Store next macro before freeing the current one

        free(current->name); // Free dynamically allocated macro name

        // Free each line stored in the macro content
        for (int i = 0; i < current->line_count; i++) {
            free(current->content[i]);
        }
        free(current->content);  // Free the content array itself
        free(current);           // Free the macro struct

        current = next;  // Move to the next macro in the list
    }
    head = NULL;   // Reset the head pointer to NULL after cleanup
}

int is_valid_macro_name(char *macro_name, const char *filename, int line_count)
{
    int valid = 0;
    // The line contains unnecessary characters.
    if (strchr(macro_name, SPACE) != NULL)
    {
        print_syntax_error(ERROR_CODE_15, filename, line_count);
        valid = 1;
    }

    int operation_count = SUM_OPERATIONS;
    // check if mcro name equals to op_word
    for (int i = 0; i < operation_count; i++)
    {
        if (strcmp(macro_name, operation_list[i].operation_name) == 0)
        {
            print_syntax_error(ERROR_CODE_14, filename, line_count);
            valid = 1;
            break;
        }
    }
    // check if mcro name equals to directive_word
    for (int i = 0; i < directive_count; i++)
    {
        if (strcmp(macro_name, directive_words[i]) == 0)
        {
            print_syntax_error(ERROR_CODE_13, filename, line_count);
            valid = 1;
        }
    }
    return valid;
}

int is_valid_macro_end(char *line, const char *filename, int line_count) {
    char *ptr = line + 7; // Move pointer to the character right after 'mcroend'

    // Check if there is any extra text after 'mcroend'
    if (*ptr != NULL_CHAR && *ptr != NULL_CHAR) {
        print_syntax_error(ERROR_CODE_16, filename , line_count); // Print syntax error for extra characters
        return 1; // Invalid macro end
    }

    return 0; // Valid macro end (no extra text)

}

