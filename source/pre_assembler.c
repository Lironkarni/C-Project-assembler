#include "../headers/pre_assembler.h"
#include "../headers/globals.h"
#include "../headers/error.h"
#include "../headers/macro.h"
#include "../headers/utils.h"


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
        print_system_error(ERROR_CODE_3);
        return;
    }
    strncpy(new_filename, filename, len - 3);
    new_filename[len - 3] = '\0';
    strcat(new_filename, ".am");

    FILE *input_file = fopen(filename, "r");
    FILE *output_file = fopen(new_filename, "w");
    if (!input_file || !output_file)
    {
        print_system_error(ERROR_CODE_4);
        if (input_file) fclose(input_file);
        free(new_filename);
        return;
    }

    char line[MAX_LINE_LEN];
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
        if (line[0] == COMMENT) {
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
                print_system_error(ERROR_CODE_3);
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
