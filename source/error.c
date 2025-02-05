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
    {ERROR_CODE_9, "label name is invalid, cannot be assembly instuction name"},
    {ERROR_CODE_10, "label name is invalid, label name must start with alphabetic char"},
    {ERROR_CODE_11, "label name is invalid, more than 31 chars"},
    {ERROR_CODE_12, "label name is invalid, label should contain alphabetic and digits only"},
    {ERROR_CODE_13, ""},
    {ERROR_CODE_14, "MACRO NAME IS INVALID (CANNOT BE AN INSTRUCTION NAME)."},
    {ERROR_CODE_15, "EXTRA CHARACTERS FOUND IN MACRO OPEN LINE."},
    {ERROR_CODE_16, "EXTRA CHARACTERS FOUND IN MACRO CLOSE LINE."}
};

int FOUND_ERROR_IN_PRE_ASSEMBLER = 0;
int FOUND_ERROR_IN_FIRST_PASS=0;

void print_system_error(int error_code){
    if(error_code==ERROR_CODE_4){
        FOUND_ERROR_IN_FIRST_PASS=1;
    }
    printf("ERROR: code %d | %s\n",error_code, errors[error_code].err_message);
}


void print_syntax_error(int error_code, const char* file_name, int line_number)
{
    if (error_code==ERROR_CODE_16 ||error_code==ERROR_CODE_14||error_code==ERROR_CODE_15 ){
        FOUND_ERROR_IN_PRE_ASSEMBLER = 1;
    }
    printf("ERROR: code %d | in file: %s | at line: %d \n%s\n", error_code, file_name, line_number, errors[error_code].err_message);
}

