#include "../headers/pre_assembler.h"
#include "../headers/globals.h"
#include "../headers/error.h"
#include "../headers/macro.h"
#include "../headers/utils.h"



void pre_assembler(const char *filename)
{
    const char *extension = ".as";
    size_t len = strlen(filename);

    // Check if the file has a .as extension
    if (len < 3 || strcmp(filename + len - 3, extension) != 0)
    {
        printf("File %s does not have a .as extension. Skipping.\n", filename);
        return;
    }

    // Allocate memory for the new .am file name
    char *new_filename = (char *)malloc(strlen(filename) + 4);
    if (!new_filename) {
        print_system_error(ERROR_CODE_3); // Memory allocation failure
        return;
    }

    // Create the .am file name
    strncpy(new_filename, filename, len - 3);
    new_filename[len - 3] = NULL_CHAR;
    strcat(new_filename, ".am");

    FILE *input_file = fopen(filename, "r");
    FILE *output_file = fopen(new_filename, "w");

    // Validate file opening
    if (!input_file || !output_file)
    {
        print_system_error(ERROR_CODE_4); // Failed to open input or output file
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

    // Process the file line by line
    while (fgets(line, sizeof(line), input_file))
    {
        line_count++;
        remove_extra_spaces(line);  // Clean extra spaces
        line[strcspn(line, "\n")] = NULL_CHAR; // Remove newline character
        // Skip comment lines
        if (line[0] == COMMENT) {
            continue;
        }

        // Skip empty lines
        if (is_empty_line(line)) {
            continue; 
        }

        // Detect start of a macro
        if (strncmp(line, "mcro ", 5) == 0) { 
            inside_macro = 1;
            new_line_count = 0;
            sscanf(line + 5, "%[^\n]", macro_name);

            // Validate macro name
            if(is_valid_macro_name(macro_name ,filename, line_count)!= 0){
                inside_macro = 0;
                continue;
            }
            macro_content = NULL;
        } 

        // Detect end of a macro
        else if ( strncmp(line, "mcroend", 7) == 0) {
            if (!inside_macro){
               print_syntax_error(ERROR_CODE_43, filename, line_count);
            } 
            //add macro only if macro_end is valid
            if (is_valid_macro_end(line,filename, line_count)==0){
                add_macro(macro_name, macro_content, new_line_count);
            }
           
            inside_macro = 0;
            
            // Free temporary macro lines memory
            for (int i = 0; i < new_line_count; i++) {
                free(macro_content[i]);
            }
            free(macro_content);
        } 

        // Store lines inside macro definition
        else if (inside_macro) { 
            macro_content = (char **)realloc(macro_content, (new_line_count + 1) * sizeof(char *));
            if (!macro_content) {
                print_system_error(ERROR_CODE_3); // Memory allocation failure
                exit(1);
            }
            macro_content[new_line_count] = copy_name(line);  // Store line in macro content
            if (!macro_content[new_line_count]) { //alloacte failed
                for (int i = 0; i < new_line_count; i++) {
                    free(macro_content[i]);
                }
                free(macro_content);
                print_system_error(ERROR_CODE_3);
                exit(1);
            }

            new_line_count++;
        } 

        // Handle normal lines or macro usage
        else { 
            Macro *macro = find_macro(line);
            if (macro) { 
                // Expand macro content into the output file
                for (int i = 0; i < macro->line_count; i++) {
                    fputs(macro->content[i], output_file);
                    fputc('\n', output_file);
                }
            } 
            else { 
                // Write regular line to the output
                fputs(line, output_file);
                fputc('\n', output_file);
            }
        }
    }
    printf("File %s processed and saved to %s\n", filename, new_filename);

    fclose(input_file);
    fclose(output_file);
    free(new_filename); // Clean up allocated memory
}
