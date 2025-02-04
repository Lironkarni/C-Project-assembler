#include "../headers/error.h"
#include "../headers/globals.h"

error errors[]={
    {ERROR_CODE_0, "NO ERROR FOUND"},
    {ERROR_CODE_1, "NO FILES ENTERED, CAN'T RUN PROGRAM"},
    {ERROR_CODE_2, ""},
    {ERROR_CODE_3, "FAILED TO ALLOCATE MEMORY"},
    {ERROR_CODE_4, "FAIL TO OPEN FILE"},
    {ERROR_CODE_5, ""},
    {ERROR_CODE_6, ""},
    {ERROR_CODE_7, ""},
    {ERROR_CODE_8, "THE LINE IS TOO LONG, MORE THAN CHARACTERS"},
    {ERROR_CODE_9, "MACRO NAME IS INVALID (CANNOT BE AN INSTRUCTION NAME)."},
    {ERROR_CODE_10, "EXTRA CHARACTERS FOUND IN MACRO OPEN LINE."},
    {ERROR_CODE_11, "EXTRA CHARACTERS FOUND IN MACRO CLOSE LINE."}

};

int FOUND_ERROR_IN_PRE_ASSEMBLER = 0;

void print_system_error(int error_code){
    printf("ERROR: code %d | %s\n",error_code, errors[error_code].err_message);
}


void print_syntax_error(int error_code, const char* file_name, int line_number)
{
    if (error_code==ERROR_CODE_9 ||error_code==ERROR_CODE_10||error_code==ERROR_CODE_11 ){
        FOUND_ERROR_IN_PRE_ASSEMBLER = 1;
    }
    printf("ERROR: code %d | in file: %s | at line: %d \n%s\n", error_code, file_name, line_number, errors[error_code].err_message);
}

