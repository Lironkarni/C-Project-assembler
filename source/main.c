#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/utils.h"

int main(int argc, char *argv[]) {
    //check if the user added one or more assembly files .as
    if (argc < 2) {
        printf("Usage: %s <filename (without extension1)>\n", argv[0]);
        return 1;
    }

    //work on the files one by one
    for (int i = 1; i < argc; i++) {

        char *as_file = load_file(argv[i]);
        if (as_file != NULL) {
            // יוצרים את הקובץ .am
            pre_assembler(as_file);
            free(as_file);

            // בודקים את הקובץ .am ומייצרים קובץ .temp
            char filename_am[256];
            snprintf(filename_am, sizeof(filename_am), "test-files/%s.am", argv[i]);
            check_the_file(filename_am);
        } else {
            printf("in main not found\n");
            printf("File %s.as not found.\n", argv[i]);
        }
        free_macros();
        //hi
        
    }

    return 0;
}
