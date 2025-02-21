#ifndef ERROR_H
#define ERROR_H

typedef struct error
{
	int err_code;
	char* err_message;
}error;

typedef enum error_codes
{
    ERROR_CODE_0,
	ERROR_CODE_1,
	ERROR_CODE_2,
	ERROR_CODE_3,
    ERROR_CODE_4,
    ERROR_CODE_5,
    ERROR_CODE_6,
    ERROR_CODE_7,
    ERROR_CODE_8,
    ERROR_CODE_9,
    ERROR_CODE_10,
    ERROR_CODE_11,
    ERROR_CODE_12,
    ERROR_CODE_13,
    ERROR_CODE_14,
    ERROR_CODE_15,
    ERROR_CODE_16,
    ERROR_CODE_17,
    ERROR_CODE_18,
    ERROR_CODE_19,
    ERROR_CODE_20,
    ERROR_CODE_21,
    ERROR_CODE_22,
    ERROR_CODE_23,
    ERROR_CODE_24,
    ERROR_CODE_25,
    ERROR_CODE_26,
    ERROR_CODE_27,
    ERROR_CODE_28,
    ERROR_CODE_29,
    ERROR_CODE_30,
    ERROR_CODE_31,
    ERROR_CODE_32,
    ERROR_CODE_33,
    ERROR_CODE_34,
    ERROR_CODE_35


}error_codes;

extern int FOUND_ERROR_IN_PRE_ASSEMBLER;
extern int FOUND_ERROR_IN_FIRST_PASS;


void print_system_error(int error_code);

void print_syntax_error(int error_code, const char* file_name, int line_number);

#endif