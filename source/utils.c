#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../headers/utils.h"
#include "../headers/error.h"
#include "../headers/process_input.h"

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

// הסרת רווחים מיותרים משורה
void remove_extra_spaces(char *line)
{
    int j = 0;
    int in_space = 0;
    int i = 0;

    while (line[i] == ' ')
    {
        i++;
    }

    for (; line[i] != '\0' && line[i] != '\n' ; i++) {
        if (line[i] != ' ') {
            line[j++] = line[i];
            in_space = 0;
        }
        else if (!in_space)
        {
            line[j++] = ' ';
            in_space = 1;
        }
    }
    if(line[j-1] == ' '){
        line[j-1] = '\0';
    }else{
        line[j] = '\0';
    }
    
}

int is_empty_line(const char *line)
{
    while (*line)
    {
        if (!isspace((unsigned char)*line))
        {
            return 0; // השורה אינה ריקה
        }
        line++;
    }
    return 1; // השורה ריקה או מכילה רק רווחים וטאבים
}

// פונקציה לעיבוד קובץ עם מקרואים
void pre_assembler(const char *filename)
{
    const char *extension = ".as";
    size_t len = strlen(filename);
    if (len < 3 || strcmp(filename + len - 3, extension) != 0)
    {
        printf("File %s does not have a .as extension. Skipping.\n", filename);
        return;
    }

    char *new_filename = (char *)malloc(strlen(filename) + 4);
    if (!new_filename) {
        printf("Memory allocation failed.\n");
        return;
    }
    strncpy(new_filename, filename, len - 3);
    new_filename[len - 3] = '\0';
    strcat(new_filename, ".am");

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

    while (fgets(line, sizeof(line), input_file))
    {
        line_count++;
        remove_extra_spaces(line);
        line[strcspn(line, "\n")] = '\0'; // הסרת תו סוף שורה

         // 🔹 בדיקת הערות – אם השורה מתחילה ב-";" דלג עליה
        if (line[0] == ';') {
            continue;
        }

        // בדיקה אם השורה ריקה
        if (is_empty_line(line)) {
            continue; // דילוג על שורות ריקות
        }

        if (strncmp(line, "mcro ", 5) == 0) { 
            inside_macro = 1;
            new_line_count = 0;
            sscanf(line + 5, "%[^\n]", macro_name);

            if(is_valid_macro_name(macro_name ,filename, line_count)!= 0){
                continue;
            }
            macro_content = NULL;
        } 
        else if (inside_macro && strncmp(line, "mcroend", 7) == 0) { 
            //add macro only if macro_end is valid
            if (is_valid_macro_end(line,filename, line_count)==0){
                add_macro(macro_name, macro_content, new_line_count);
            }
           
            inside_macro = 0;
            // שחרור הזיכרון הזמני ששימש לאחסון השורות
            for (int i = 0; i < new_line_count; i++) {
                free(macro_content[i]);
            }
            free(macro_content);
        } 
        else if (inside_macro) { 
            macro_content = (char **)realloc(macro_content, (new_line_count + 1) * sizeof(char *));
            if (!macro_content) {
                printf("Memory allocation failed while storing macro content.\n");
                exit(1);
            }
            macro_content[new_line_count] = strdup(line);
            new_line_count++;
        } 
        else { 
            Macro *macro = find_macro(line);
            if (macro) { 
                for (int i = 0; i < macro->line_count; i++) {
                    fputs(macro->content[i], output_file);
                    fputc('\n', output_file);
                }
            } 
            else { 
                fputs(line, output_file);
                fputc('\n', output_file);
            }
        }
    }
    printf("File %s processed and saved to %s\n", filename, new_filename);

    fclose(input_file);
    fclose(output_file);
    free(new_filename);
}

// טעינת קובץ עם הקצאה דינאמית
char *load_file(char *filename) {

    char *filename_with_extension = (char *)malloc(strlen("test-files/") + strlen(filename) + strlen(".as") + 1);
    if (!filename_with_extension) {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
        sprintf(filename_with_extension, "test-files/%s.as", filename);

    FILE *file = fopen(filename_with_extension, "r");
    if (!file)
    {
        printf("Error: File %s does not exist. Skipping.\n", filename_with_extension);
        free(filename_with_extension);
        return NULL;
    }
    fclose(file);
    return filename_with_extension;
    }

// פונקציה שבודקת את מספר השורות בקובץ .am ויוצרת קובץ .temp עם מספר השורות
void check_the_file(const char *filename_ad)
{
    FILE *file = fopen(filename_ad, "r");
    if (!file)
    {
        printf("Error: Unable to open %s for reading.\n", filename_ad);
        return;
    }

    int line_count = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file))
    {
        line_count++;
    }
    fclose(file);

    char *filename_ah = (char *)malloc(strlen(filename_ad) + 3);
    if (!filename_ah) {
        printf("Memory allocation failed.\n");
        return;
    }

    sprintf(filename_ah, "%s.ah", filename_ad);
    
    FILE *file_ah = fopen(filename_ah, "w");
    if (!file_ah)
    {
        printf("Error: Unable to open %s for writing.\n", filename_ah);
        free(filename_ah);
        return;
    }

    fprintf(file_ah, "Number of lines: %d\n", line_count);
    fclose(file_ah);
    free(filename_ah);
    
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
    if (strchr(macro_name, ' ') != NULL) {
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

    if (*ptr != '\0' && *ptr != '\n') {
        print_syntax_error(ERROR_CODE_16, filename , line_count);
        return 1;
    }

    return 0; // אין שגיאה

}

void delete_am_file(const char *filename) {

    size_t len = strlen(filename) + strlen("test-files/") + strlen(".am") + 1;

    char *am_filename = (char *)malloc(len);
    if (am_filename == NULL) {
        perror("Memory allocation failed");
        return;
    }

    snprintf(am_filename, len, "test-files/%s.am", filename);

    if (remove(am_filename) == 0) {
        printf("File %s deleted successfully.\n", am_filename);
    } else {
        perror("Error deleting file");
    }

    free(am_filename);
}

Line *create_line(char* temp_line,char* file,int line_number){
    Line *new_line;
    new_line=(Line *)malloc(sizeof(Line));
    if(new_line==NULL){
        print_system_error(ERROR_CODE_3);
        return NULL;
    }
    
    new_line->file_name=(char *)malloc(sizeof(file)+1);
    if(new_line->file_name==NULL){
        print_system_error(ERROR_CODE_3);
       return NULL;
    }
    strcpy(new_line->file_name,file);
    new_line->data=(char *)malloc(sizeof(temp_line)+1);
    if(new_line->data==NULL){
        print_system_error(ERROR_CODE_3);
       return NULL;
    }
    strcpy(new_line->data,temp_line);
    new_line->line_number=line_number;

    return new_line;
}
