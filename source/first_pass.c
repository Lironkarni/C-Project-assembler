#include "../headers/globals.h"
#include "../headers/utils.h"

/*start the first pass*/
void first_pass(char* file)
{
    const char *filename_with_extension;
    filename_with_extension = load_file(file);
    printf("start first pass %s", filename_with_extension);

    printf("hi");
    printf("hi");

}
