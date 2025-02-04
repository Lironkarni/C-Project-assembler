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
    ERROR_CODE_11

}error_codes;

extern int FOUND_ERROR_IN_PRE_ASSEMBLER;


void print_system_error(int error_code);

void print_syntax_error(int error_code, const char* file_name, int line_number);
