#include "../headers/error.h"
#include "../headers/globals.h"

error errors[]={
    {ERROR_CODE_0, "No error found"},
    {ERROR_CODE_1, "No files entered, can't run program"},
    {ERROR_CODE_2, ""},
    {ERROR_CODE_3, "failed to allocate memory"},
    {ERROR_CODE_4, "fail to open file"},
    {ERROR_CODE_5, ""},
    {ERROR_CODE_6, ""},
    {ERROR_CODE_7, ""},
    {ERROR_CODE_8, "the line is too long, more than characters"}

};

void print_system_error(int error_code){
    printf("Error: code %d | %s\n",error_code, errors[error_code].err_message);
}


void print_syntax_error(int error_code, char* file_name, int line_number)
{
    printf("Error: code: %d | file: %s | at line: %d | %s", error_code, file_name, line_number, errors[error_code].err_message);
}

