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
    {ERROR_CODE_8, "the line is too long, more than 80 characters"},
    {ERROR_CODE_9, "label name is invalid, cannot be assembly instuction name"},
    {ERROR_CODE_10, "label name is invalid, label name must start with alphabetic char"},
    {ERROR_CODE_11, "label name is invalid, more than 31 chars"},
    {ERROR_CODE_12, "label name is invalid, label should contain alphabetic and digits only"}

};

void print_system_error(int error_code){
    printf("Error: code %d | %s\n",error_code, errors[error_code].err_message);
}


void print_syntax_error(int error_code, char* file_name, int line_number)
{
    printf("Error: code: %d | file: %s | at line: %d | %s", error_code, file_name, line_number, errors[error_code].err_message);
}

