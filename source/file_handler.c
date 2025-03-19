#include "../headers/file_handler.h"
#include "../headers/error.h"

void delete_am_file(const char *filename)
{

    /* Calculate the required length for the full file path */
    size_t len = strlen(filename) + strlen("test-files/") + strlen(".am") + 1;

    /* Allocate memory for the full file path */
    char *am_filename = (char *)malloc(len);
    if (am_filename == NULL)
    {
        print_system_error(ERROR_CODE_3); /* Memory allocation failed */
        return;
    }

    /* Create the full .am file path */
    snprintf(am_filename, len, "test-files/%s.am", filename);

    /* Attempt to delete the .am file */
    if (remove(am_filename) == 0)
    {
        printf("File %s deleted successfully.\n", am_filename);
    }
    else
    {
        print_system_error(ERROR_CODE_7); /* File deletion failed */
    }

    /* Free the allocated memory for the file path */
    free(am_filename);
}


char *load_file(char *filename)
{
    /* Allocate memory for the full file path */
    char *filename_with_extension = (char *)malloc(strlen("test-files/") + strlen(filename) + strlen(".as") + 1);
    if (!filename_with_extension)
    {
        print_system_error(ERROR_CODE_3);
        return NULL;
    }

    /* Build the full file path */
    sprintf(filename_with_extension, "test-files/%s.as", filename);

    /* Try to open the file to ensure it exists */
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

void make_ext_file(const char *filename, ext_list *ext_table_head) {
    ext_list *current;
    char ext_filename[256];
    size_t len = strlen(filename);

    /* Build .ext filename based on input */
    if (len > 3 && strcmp(filename + len - 3, ".am") == 0) {
        strncpy(ext_filename, filename, len - 3); 
        ext_filename[len - 3] = '\0'; 
    } else {
        strncpy(ext_filename, filename, sizeof(ext_filename) - 1);
        ext_filename[sizeof(ext_filename) - 1] = '\0';
    }
    strcat(ext_filename, ".ext"); 

    FILE *file = fopen(ext_filename, "w");
    if (!file) {
        print_system_error(ERROR_CODE_4);
        return;
    }

    /* Count number of extern entries */
    current = ext_table_head;
    int count = 0;
    while (current) {
            count++;
        current = current->next;
    }

    /* Allocate array to reverse externs order */
    ext_list **externals = (ext_list **)malloc(count * sizeof(ext_list *));
    if (!externals) {
        print_system_error(ERROR_CODE_3);
        fclose(file);
        return;
    }

    /* Populate externs array */
    current = ext_table_head;
    int i = 0;
    while (current) {
        externals[i++] = current;
        current = current->next;
    }

    /* Write externs in reverse order */
    for (int j = i - 1; j >= 0; j--) {
        fprintf(file, "%-6s %07d\n", externals[j]->label_name, externals[j]->address);
    }

    free(externals);
    fclose(file);
    printf("File %s created successfully.\n", ext_filename);

}

void make_ent_file(const char *filename, Symbol *symbol_table_head) {
    char ent_filename[256];
    size_t len = strlen(filename);

    /* Build .ent filename */
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
        print_system_error(ERROR_CODE_4);
        return;
    }

    /* Count number of entry symbols */
    int count = 0;
    Symbol *current = symbol_table_head;
    while (current) {
        if (current->type == 2) { // ENTRY type
            count++;
        }
        current = current->next;
    }

    /* If no entries, close file and return */
    if (count == 0) { 
        fclose(file);
        printf("File %s created successfully, but no entries were written.\n", ent_filename);
        return;
    }

    /* Allocate array to reverse order */
    Symbol **symbols = (Symbol **)malloc(count * sizeof(Symbol *));
    if (!symbols) {
        print_system_error(ERROR_CODE_3);
        fclose(file);
        return;
    }

    /* Collect entry symbols */
    current = symbol_table_head;
    int i = 0;
    while (current) {
        if (current->type == 2) {
            symbols[i++] = current;
        }
        current = current->next;
    }

    /* Write entries in reverse order */
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

    /* Build .ob filename */
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
        print_system_error(ERROR_CODE_4);
        return;
    }

    /* Write code and data size line */
    fprintf(file, "     %d  %d\n", ic-INIT_MEMORY, dc);

    /* Write code segment */
    for (int i = INIT_MEMORY; i < ic; i++) {
        uint32_t full_word = 0;
        memcpy(&full_word, &code_image[i], 3); // Copy 3 bytes (24 bits)

        fprintf(file, "%07d  %06X\n", i, full_word);
    }

    /* Write data segment */
    for (int i = 0; i < dc; i++) {
        fprintf(file, "%07d  %06X\n", i + ic, data_image[i].data);
    }

    fclose(file);
    printf("File %s created successfully.\n", ob_filename);
}