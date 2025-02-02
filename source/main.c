#include "../headers/utils.h"
#include "../headers/globals.h"
#include "../headers/error.h"
#include "../headers/first_pass.h"

int main(int argc, char *argv[]) {
    int i;
    const char *as_file;
    if (argc < MIN_PARAM) {
        // printf("Usage: %s <filename (without extension)>\n", argv[0]);
        print_system_error(ERROR_CODE_1);
        return 1;
    }
    
    //work on the files one by one
    for (i = 1; i < argc; i++) {


        as_file = load_file(argv[i]);
        if (as_file != NULL) {
            // יוצרים את הקובץ .am
            pre_assembler(as_file);
            free(as_file);

            // בודקים את הקובץ .am ומייצרים קובץ .temp
            char filename_ad[256];
            snprintf(filename_ad, sizeof(filename_ad), "test-files/%s.am", argv[i]);
            check_the_file(filename_ad);
            first_pass(filename_ad);
        } else {
            printf("in main not found\n");
            printf("File %s.as not found.\n", argv[i]);
        }
        free_macros();
        
    }

    return 0;
}
