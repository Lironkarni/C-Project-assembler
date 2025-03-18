#include "../headers/utils.h"
#include "../headers/globals.h"
#include "../headers/error.h"
#include "../headers/first_pass.h"

int main(int argc, char *argv[]) {
    int i;
    char *as_file;
    if (argc < MIN_PARAM) {
        print_system_error(ERROR_CODE_1);
        return 1;
    }
    
    //work on the files one by one
    for (i = 1; i < argc; i++) {
        as_file = load_file(argv[i]);
        if (as_file != NULL) {
            // create .am file
            pre_assembler(as_file);
            free(as_file);

            //if found error in pre-assembler - continue to the next as file (without first\seccond pass)
            if (FOUND_ERROR_IN_PRE_ASSEMBLER){
                delete_am_file(argv[i]);
                FOUND_ERROR_IN_PRE_ASSEMBLER = 0;
                continue;
            }

            char filename_am[256];
            snprintf(filename_am, sizeof(filename_am), "test-files/%s.am", argv[i]);
            /*start the first pass */
            first_pass(filename_am);
            if (FOUND_ERROR_IN_FIRST_PASS){
                FOUND_ERROR_IN_FIRST_PASS = 0;
                continue;
            }
        
        } else {
            printf("in main not found\n");
            printf("File %s.as not found.\n", argv[i]);
        }
        free_macros();
    }
    return 0;
}
