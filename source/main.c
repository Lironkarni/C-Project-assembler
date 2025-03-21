#include "../headers/utils.h"
#include "../headers/globals.h"
#include "../headers/error.h"
#include "../headers/first_pass.h"
#include "../headers/pre_assembler.h"
#include "../headers/macro.h"
#include "../headers/file_handler.h"
#include "../headers/label.h"


int main(int argc, char *argv[]) {
    int i;
    char *as_file;

    // Check if enough arguments (input files) were provided
    if (argc < MIN_PARAM) {
        print_system_error(ERROR_CODE_1); // Error: No input files
        return 1;
    }
    
    //work on the files one by one
    for (i = 1; i < argc; i++) {
        as_file = load_file(argv[i]);  // Load the .as file
    // create .am file
        if (as_file != NULL) {
            pre_assembler(as_file); // Run the pre-assembler phase (handles macros)
            free(as_file);          // Free allocated memory for the file path

            //if found error in pre-assembler - continue to the next as file (without first\seccond pass)
            if (FOUND_ERROR_IN_PRE_ASSEMBLER){
                delete_am_file(argv[i]);            // Delete the .am file generated
                FOUND_ERROR_IN_PRE_ASSEMBLER = 0;   // Reset error flag
                continue;
            }

            char filename_am[256];
            snprintf(filename_am, sizeof(filename_am), "test-files/%s.am", argv[i]);
            
            first_pass(filename_am);   // Start first pass (parsing, symbol table build)

            // If errors found in the first pass, skip to next file
            if (FOUND_ERROR_IN_FIRST_PASS){
                FOUND_ERROR_IN_FIRST_PASS = 0; // Reset error flag for next round
                continue;
            }
        
        } else {
            printf("File %s.as not found.\n", argv[i]);
        }

        free_macros();  // Free all macros stored in memory
        free_symbols();// Free all symbols in memory
        free_externs();// Free all externs in memory
        
        IC = 100;   // Initializing the Instruction counter
        DC = 0;     // Initializing the Data counter
    }
    return 0;
}
