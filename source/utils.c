#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../headers/utils.h"
#include "../headers/error.h"
#include "../headers/process_input.h"
#include "../headers/globals.h"

Macro *head = NULL;  // Pointer to the head of the Macro list


void add_macro(const char *name, char **content, int line_count) {
    /* Allocate memory for the new macro */
    Macro *new_macro = (Macro *)malloc(sizeof(Macro));
    if (!new_macro) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    /* Store the macro name */
    new_macro->name = strdup(name);
    new_macro->line_count = line_count;
    new_macro->next = NULL;

    /* Allocate memory for macro content lines */
    new_macro->content = (char **)malloc(line_count * sizeof(char *));
    if (!new_macro->content) {
        printf("Memory allocation failed!\n");
        free(new_macro->name);
        free(new_macro);
        exit(1);
    }

    /* Duplicate each line of the macro content */
    for (int i = 0; i < line_count; i++) {
        new_macro->content[i] = strdup(content[i]);
    }

    /* Add the new macro to the end of the linked list */
    if (!head) {
        head = new_macro;  // If the list is empty, new macro is the head
    } else {
        Macro *current = head;
        while (current->next) {
            current = current->next; // Traverse to the end of the list
        }
        current->next = new_macro; // Append the new macro
    }
}

Macro *find_macro(const char *name) {
    Macro *current = head;

    /* Iterate through the macro list */
    while (current) {
        /* Compare the current macro name with the target name */
        if (strcmp(current->name, name) == 0) {
            return current; // Macro found
        }
        current = current->next; // Move to the next macro
    }
    return NULL; // Macro not found
}

void remove_extra_spaces(char *line)
{
    int j = 0;   // Index for writing cleaned characters
    int in_space = 0;    // Flag to track consecutive spaces
    int i = 0;    // Index for reading characters

    /* Skip leading spaces */
    while (line[i] == ' ')
    {
        i++;
    }

    /* Process the line character by character */
    for (; line[i] != '\0' && line[i] != '\n' ; i++) {
        if (line[i] != ' ') {
            line[j++] = line[i]; // Copy non-space character
            in_space = 0;
        }
        else if (!in_space)
        {
            line[j++] = ' '; // Copy a single space
            in_space = 1;
        }
         // If already in space, skip additional spaces
    }
    /* Remove trailing space if present */
    if(line[j-1] == ' '){
        line[j-1] = '\0';
    }else{
        line[j] = '\0';
    }
    
}

int is_empty_line(const char *line)
{
    /* Iterate through each character in the line */
    while (*line)
    {
        /* If a non-whitespace character is found, the line is not empty */
        if (!isspace((unsigned char)*line))
        {
            return 0; // Line is not empty
        }
        line++;
    }
    return 1; // Line is empty or contains only spaces/tabs
}

void pre_assembler(const char *filename)
{
    const char *extension = ".as";
    size_t len = strlen(filename);

    /* Check if the file has a .as extension */
    if (len < 3 || strcmp(filename + len - 3, extension) != 0)
    {
        printf("File %s does not have a .as extension. Skipping.\n", filename);
        return;
    }

    /* Create the new filename with .am extension */
    char *new_filename = (char *)malloc(strlen(filename) + 4);
    if (!new_filename) {
        printf("Memory allocation failed.\n");
        return;
    }
    strncpy(new_filename, filename, len - 3);
    new_filename[len - 3] = '\0';
    strcat(new_filename, ".am");

    /* Open input .as file and output .am file */
    FILE *input_file = fopen(filename, "r");
    FILE *output_file = fopen(new_filename, "w");
    if (!input_file || !output_file)
    {
        perror("Failed to open files");
        if (input_file) fclose(input_file);
        free(new_filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    char macro_name[50];
    char **macro_content = NULL;
    int inside_macro = 0;
    int line_count = 0;
    int new_line_count = 0;

    /* Process each line from the input file */
    while (fgets(line, sizeof(line), input_file))
    {
        line_count++;
        remove_extra_spaces(line); // Clean extra spaces
        line[strcspn(line, "\n")] = '\0';  // Remove newline character

        /* Skip comment lines */
        if (line[0] == ';') {
            continue;
        }

        /* Skip empty lines */
        if (is_empty_line(line)) {
            continue; 
        }

        /* Handle macro start */
        if (strncmp(line, "mcro ", 5) == 0) { 
            inside_macro = 1;
            new_line_count = 0;
            sscanf(line + 5, "%[^\n]", macro_name);

            /* Validate macro name */
            if(is_valid_macro_name(macro_name ,filename, line_count)!= 0){
                continue;
            }
            macro_content = NULL;
        } 

        /* Handle macro end */
        else if (inside_macro && strncmp(line, "mcroend", 7) == 0) { 
            if (is_valid_macro_end(line,filename, line_count)==0){
                add_macro(macro_name, macro_content, new_line_count); // Store the macro
            }
           
            inside_macro = 0;
            
            /* Free temporary storage for macro lines */
            for (int i = 0; i < new_line_count; i++) {
                free(macro_content[i]);
            }
            free(macro_content);
        } 

        /* Store lines inside the macro definition */
        else if (inside_macro) { 
            macro_content = (char **)realloc(macro_content, (new_line_count + 1) * sizeof(char *));
            if (!macro_content) {
                print_system_error(ERROR_CODE_3); // Memory allocation error
                exit(1);
            }
            macro_content[new_line_count] = strdup(line); // Store macro line
            new_line_count++;
        } 
        /* Process normal lines or expand macros if found */
        else { 
            Macro *macro = find_macro(line);
            if (macro) { 
                /* Write expanded macro content */
                for (int i = 0; i < macro->line_count; i++) {
                    fputs(macro->content[i], output_file);
                    fputc('\n', output_file);
                }
            } 
            else { 
                /* Write the original line */
                fputs(line, output_file);
                fputc('\n', output_file);
            }
        }
    }
    printf("File %s processed and saved to %s\n", filename, new_filename);

    /* Cleanup */
    fclose(input_file);
    fclose(output_file);
    free(new_filename);
}

char *load_file(char *filename) {

    /* Allocate memory for the full file path with .as extension */
    char *filename_with_extension = (char *)malloc(strlen("test-files/") + strlen(filename) + strlen(".as") + 1);
    if (!filename_with_extension) {
        print_system_error(ERROR_CODE_3); // Memory allocation failed
        return NULL;
    }
        /* Build the full path string */
        sprintf(filename_with_extension, "test-files/%s.as", filename);

    /* Check if the file exists and can be opened */
    FILE *file = fopen(filename_with_extension, "r");
    if (!file)
    {
        print_system_error(ERROR_CODE_2); // File does not exist
        free(filename_with_extension);
        return NULL;
    }
    fclose(file); // Close the file after validation
    return filename_with_extension; // Return the allocated file path
    }

void free_macros() {
    Macro *current = head;

    /* Iterate and free each macro node */
    while (current) {
        Macro *next = current->next;

        free(current->name); // Free macro name
        /* Free each line of the macro content */
        for (int i = 0; i < current->line_count; i++) {
            free(current->content[i]);
        }
        free(current->content); // Free the content array
        free(current);          // Free the macro structure itself

        current = next; // Move to the next macro
    }
    head = NULL;  // Reset the head pointer
}

int is_valid_macro_name(char *macro_name ,const char *filename , int line_count) {
    int valid = 0;
    /* Check if the macro name contains spaces - invalid */
    if (strchr(macro_name, SPACE) != NULL) {
        print_syntax_error(ERROR_CODE_15, filename ,line_count); // Extra characters found
        valid = 1;
    }

    int operation_count = SUM_OPERATIONS;
    /* Check if the macro name matches any operation name - invalid */
    for (int i = 0; i < operation_count; i++) {
        if (strcmp(macro_name, operation_list[i].operation_name) == 0) {
            print_syntax_error(ERROR_CODE_14, filename, line_count); // Macro name cannot be operation name
            valid = 1;
            break;  
        }
    }
    /* Check if the macro name matches any directive keyword - invalid */
    for (int i = 0; i < directive_count; i++) {
        if (strcmp(macro_name, directive_words[i]) == 0) {
            print_syntax_error(ERROR_CODE_13, filename , line_count); // Macro name cannot be directive
            valid = 1;
        }
    }
    return valid;  /* Return 1 if invalid, 0 if valid */
}

int is_valid_macro_end(char *line, const char *filename, int line_count) {
    /* Move the pointer to the character after 'endmacro' (7 characters) */
    char *ptr = line + 7;

    /* Check if there is any extra text after 'endmacro' */
    if (*ptr != NULL_CHAR && *ptr != NULL_CHAR) {
        print_syntax_error(ERROR_CODE_16, filename , line_count); /* Extra characters found */
        return 1;
    }

    return 0; /* No error */

}

void delete_am_file(const char *filename) {

    /* Calculate the required length for the full file path */
    size_t len = strlen(filename) + strlen("test-files/") + strlen(".am") + 1;

    /* Allocate memory for the full file path */
    char *am_filename = (char *)malloc(len);
    if (am_filename == NULL) {
        print_system_error(ERROR_CODE_3); /* Memory allocation failed */
        return;
    }

    /* Create the full .am file path */
    snprintf(am_filename, len, "test-files/%s.am", filename);

    /* Attempt to delete the .am file */
    if (remove(am_filename) == 0) {
        printf("File %s deleted successfully.\n", am_filename);
    } else {
        print_system_error(ERROR_CODE_7); /* File deletion failed */
    }

    /* Free the allocated memory for the file path */
    free(am_filename);
}

Line *create_line(char* temp_line,char* file,int line_number){
    
    Line *new_line;

    /* Allocate memory for the Line structure */
    new_line=(Line *)malloc(sizeof(Line));
    if(new_line==NULL){
        print_system_error(ERROR_CODE_3); /* Memory allocation failed */
        return NULL;
    }
    
    /* Allocate memory for storing the file name */
    new_line->file_name=(char *)malloc(strlen(file)+1);
    if(new_line->file_name==NULL){
        print_system_error(ERROR_CODE_3);
       return NULL;
    }
    
    /* Copy file name into the structure */
    strcpy(new_line->file_name,file);
    
    /* Allocate memory for storing the line data */
    new_line->data=(char *)malloc(strlen(temp_line)+1);
    
    if(new_line->data==NULL){
        print_system_error(ERROR_CODE_3);
       return NULL;
    }
    
    /* Copy line content into the structure */
    strcpy(new_line->data,temp_line);

    /* Store the line number */
    new_line->line_number=line_number;
    
    return new_line; /* Return the created Line structure */
}

int extraneous_text(char *command)
{
    int i;
    /* Iterate through the command string */
    for(i=0;*command!=NULL_CHAR;i++)
    {
        /* If a non-space, non-null character is found, it's extraneous text */
        if(command[i]!=NULL_CHAR && command[i]!=SPACE){
            return 1;  /* Extraneous text found */
        }
    }

    /* If the string is empty or contains only spaces, return 0 */
    if(i==0){
        return 0;
    }
    return 0;
}
