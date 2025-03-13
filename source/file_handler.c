#include "../headers/file_handler.h"



void make_ent_file(const char *filename, Symbol *symbol_table_head) {
    char ent_filename[256];
    size_t len = strlen(filename);

    if (len > 3 && strcmp(filename + len - 3, ".am") == 0) {
        strncpy(ent_filename, filename, len - 3); 
        ent_filename[len - 3] = '\0'; 
    } else {
        strncpy(ent_filename, filename, sizeof(ent_filename) - 1);
        ent_filename[sizeof(ent_filename) - 1] = '\0';
    }

    strcat(ent_filename, ".ent"); 

    FILE *file = fopen(ent_filename, "w");
    if (!file) {
        perror("Error opening .ent file");
        return;
    }

    int count = 0;
    Symbol *current = symbol_table_head;
    while (current) {
        if (current->type == 2) {
            count++;
        }
        current = current->next;
    }

    if (count == 0) { 
        fclose(file);
        printf("File %s created successfully, but no entries were written.\n", ent_filename);
        return;
    }

    Symbol **symbols = (Symbol **)malloc(count * sizeof(Symbol *));
    if (!symbols) {
        perror("Memory allocation failed");
        fclose(file);
        return;
    }

    current = symbol_table_head;
    int i = 0;
    while (current) {
        if (current->type == 2) {
            symbols[i++] = current;
        }
        current = current->next;
    }

    for (int j = count - 1; j >= 0; j--) {
        fprintf(file, "%-6s %07d\n", symbols[j]->name, symbols[j]->address);
    }

    free(symbols);
    fclose(file);
    printf("File %s created successfully.\n", ent_filename);
}

void make_ob_file(const char *filename, code_word *code_image, int ic, data_word *data_image, int dc) {
    char ob_filename[256];
    size_t len = strlen(filename);

    if (len > 3 && strcmp(filename + len - 3, ".am") == 0) {
        strncpy(ob_filename, filename, len - 3);
        ob_filename[len - 3] = '\0';
    } else {
        strncpy(ob_filename, filename, sizeof(ob_filename) - 1);
        ob_filename[sizeof(ob_filename) - 1] = '\0';
    }

    strcat(ob_filename, ".ob");

    FILE *file = fopen(ob_filename, "w");
    if (!file) {
        perror("Error opening .ob file");
        return;
    }

    fprintf(file, "     %d  %d\n", ic-100, dc);

    for (int i = 100; i < ic; i++) {
        uint32_t full_word = 0;
        memcpy(&full_word, &code_image[i], 3);

        fprintf(file, "%07d  %06X\n", i, full_word);
    }

    for (int i = 0; i < dc; i++) {
        fprintf(file, "%07d  %06X\n", i + ic, data_image[i].data);
    }

    fclose(file);
    printf("File %s created successfully.\n", ob_filename);
}

// טעינת קובץ עם הקצאה דינאמית
char *load_file(char *filename) {

    char *filename_with_extension = (char *)malloc(strlen("test-files/") + strlen(filename) + strlen(".as") + 1);
    if (!filename_with_extension) {
        print_system_error(ERROR_CODE_3);
        return NULL;
    }
        sprintf(filename_with_extension, "test-files/%s.as", filename);

    FILE *file = fopen(filename_with_extension, "r");
    if (!file)
    {
        print_system_error(ERROR_CODE_2);
        free(filename_with_extension);
        return NULL;
    }
    fclose(file);
    return filename_with_extension;
    }

void delete_am_file(const char *filename) {

        size_t len = strlen(filename) + strlen("test-files/") + strlen(".am") + 1;
    
        char *am_filename = (char *)malloc(len);
        if (am_filename == NULL) {
            print_system_error(ERROR_CODE_3);
            return;
        }
    
        snprintf(am_filename, len, "test-files/%s.am", filename);
    
        if (remove(am_filename) == 0) {
            printf("File %s deleted successfully.\n", am_filename);
        } else {
            print_system_error(ERROR_CODE_7);
        }
    
        free(am_filename);
    }
    

