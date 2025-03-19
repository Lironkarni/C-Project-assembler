#include "../headers/memory_struct.h"
#include "../headers/first_pass.h"
#include "../headers/op_list.h"
#include "../headers/error.h"

int IC = 100;   // Initializing the Instruction counter
int DC = 0;     // Initializing the Data counter

A_R_E a_r_e = {4, 2, 1};  // Initializing the addressing bits array

int get_data(Line *line, int inst_index, int **numbers)
{
    int expect_number = 1; // 0- expect number, 1- expect comma
    int capacity, count = 0;
    char *data_ptr = strstr(line->data, instruction_list[inst_index]);

    /* Move pointer past directive keyword */
    data_ptr += strlen(instruction_list[inst_index]);
    while (*data_ptr == ' ')
        data_ptr++;

    /* Allocate initial memory for numbers array */
    capacity = INIT_CAPACITY;
    *numbers = (int *)malloc(capacity * sizeof(int));
    if (*numbers == NULL)
    {
        print_system_error(ERROR_CODE_3); // Memory allocation failure
        exit(1);
    }

    /* Parse each number/comma sequence in the line */
    while (*data_ptr != NULL_CHAR)
    {
        if (expect_number)
        {
            char *end_ptr;
            /* Check if sign character is followed by a digit */
            if (*data_ptr == MINUS || *data_ptr == PLUS)
            {
                if (!isdigit(*(data_ptr + 1)))
                { 
                    print_syntax_error(22, line->file_name, line->line_number); // Invalid number syntax
                    free(*numbers);
                    return 1;
                }
            }
            else if (!isdigit(*data_ptr))
            {
                print_syntax_error(ERROR_CODE_22, line->file_name, line->line_number);
                free(*numbers);
                return 1;
            }
            /* Convert string to integer */
            int num=strtol(data_ptr, &end_ptr, DECIMAL);

            /* Reject floating point numbers */
            if (*end_ptr == '.') { 
                print_syntax_error(ERROR_CODE_41, line->file_name, line->line_number); // Decimal not allowed
                free(*numbers);
                return 1;
            }
            /* Resize numbers array if necessary */
            if (count >= capacity)
            {
                capacity *= 2;
                *numbers = (int *)realloc(*numbers, capacity * sizeof(int)); 
                if (*numbers == NULL)
                {
                    print_system_error(ERROR_CODE_3);  // Memory reallocation failure
                    exit(1);
                }
            }
            (*numbers)[count++] = num; /* Store number in array */
            expect_number = 0;  /* Next character should be comma */
            data_ptr= end_ptr;
        }
        else
        {   
            /* Ensure comma is present between numbers */
            if (*data_ptr != COMMA)
            {
                print_syntax_error(ERROR_CODE_23, line->file_name, line->line_number);  // Missing comma
                free(*numbers);
                return 1;
            }
            expect_number = 1; /* Next we expect a number */
            data_ptr++;
        }
        /* Skip spaces between numbers/commas */
        while (*data_ptr == SPACE)
            data_ptr++;
    }
    /* Check if data instruction ends incorrectly with a comma */
    if (expect_number)
    {
        print_syntax_error(ERROR_CODE_24, line->file_name, line->line_number);
        free(*numbers);
        return 1;
    }
    (*numbers)[count] = NULL_CHAR;  // Mark end of numbers array
    return 0; /* Success */
}

int get_string_data(Line *line, int inst_index, char **characters)
{
    int capacity = INIT_CAPACITY, length = 0;
    char *char_array;
    char *data_ptr = strstr(line->data, instruction_list[inst_index]);

    /* Check if directive exists in the line */
    if (!data_ptr)
    {
        print_syntax_error(ERROR_CODE_22, line->file_name, line->line_number);  // Directive missing
        return 1;
    }

    /* Move pointer past directive keyword */
    data_ptr += strlen(instruction_list[inst_index]);

    /* Skip initial spaces */
    while (*data_ptr == SPACE) // דילוג על רווחים
        data_ptr++;

    /* Check if string begins with quotation mark */
    if (*data_ptr != QUOTE)
    { 
        print_syntax_error(ERROR_CODE_20, line->file_name, line->line_number);  // String not in quotes
        return 1;
    }

    data_ptr++;  /* Skip opening quotation mark */

    /* Allocate initial memory for storing string */
    char_array = (char *)malloc(capacity * sizeof(char));
    if (!char_array)
    {
        print_system_error(ERROR_CODE_3);
        exit(1);
    }

    /* Read characters until closing quotation mark */
    while (*data_ptr && *data_ptr != QUOTE)
    {
        if (length >= capacity)
        {
            capacity *= 2;
            char_array = (char *)realloc(char_array, capacity * sizeof(char));
            if (!char_array)
            {
                print_system_error(ERROR_CODE_3);  // Memory reallocation failed
                exit(1);
            }
        }
        char_array[length++] = *data_ptr++;
    }

    /* Check for missing closing quotation mark */
    if (*data_ptr != QUOTE)
    { 
        print_syntax_error(ERROR_CODE_20, line->file_name, line->line_number);   // Missing closing quote
        free(char_array);
        return 1;
    }

    char_array[length] = NULL_CHAR;  /* Mark end of string */
    *characters = char_array;

    data_ptr++; /* Move past closing quotation mark */

    /* Skip trailing spaces */
    while (*data_ptr == SPACE)
        data_ptr++;

     /* Check for extraneous characters after the closing quotation mark */
    if (*data_ptr != NULL_CHAR)
    {
        print_syntax_error(ERROR_CODE_21, line->file_name, line->line_number);  // Unexpected characters after string
        free(char_array);
        return 1;
    }

    return 0;/* Success */
}

void add_data(data_word *data_image, int *numbers, Line *line)
{
    int numbers_size = 0;

    /* Calculate the number of integers in the numbers array */
    while (numbers[numbers_size] != '\0')
    {
        numbers_size++;
    }

    /* Check for memory overflow in the data segment */
    if (DC + numbers_size + 1 > MEM_SIZE)
    {
        // memory overflow
        print_system_error(ERROR_CODE_3); // Memory limit exceeded
        return;
    }

    /* Store each number in the data image, masking to 24 bits */
    for (int i = 0; i < numbers_size; i++)
    {
        data_image[DC].data = (uint32_t)(numbers[i] & 0xFFFFFF); // Store only lower 24 bits
        DC++;  // Increment Data Counter
    }
}

void add_string_data(data_word *data_image, char *char_array, Line *line)
{
    int length = strlen(char_array);

    /* Check for memory overflow before adding the string (including null terminator) */
    if (DC + length + 1 > MEM_SIZE)
    {
        print_system_error(ERROR_CODE_3); // Memory limit exceeded
        return;
    }

    /* Store each character of the string into the data image */
    for (int i = 0; i <= length; i++)
    {
        if (i == length)
        {
            /* Store null terminator at the end of the string */
            data_image[DC].data = 0;
        }
        else
        {
            /* Store the ASCII value of the character, keeping only the lowest 8 bits */
            data_image[DC].data = (uint32_t)(char_array[i] & 0xFF); // The lowest 8 bits only
        }
        DC++;// Increment Data Counter
    }
}

void add_to_code_image(code_word *code_image, Line *line, int num_args, int op_index, int funct, int address_method_src, int address_method_des, char *first_operand, char *second_operand)
{
    // Check for memory overflow before adding to the code image
    if(IC>=MEM_SIZE)
    {
        print_system_error(ERROR_CODE_37);  // Memory overflow error
        
        // TODO: stop the program? stop forst pass?
    }
    code_union code_u;

    // Fill the first word of the instruction
    code_image[IC].op_code = (uint8_t)(op_index & MASK);
    code_image[IC].A_R_E = (uint8_t)(a_r_e.A & MASK);
    code_image[IC].funct = (uint8_t)(funct & MASK);
    code_image[IC].source_address = (uint8_t)(address_method_src & MASK);

    // Handle source register if addressing is direct register
    if (address_method_src == 3)
    {

        int reg_src = strtol(&first_operand[1], NULL, DECIMAL); // Extract register number
        code_image[IC].source_reg = (uint8_t)(reg_src & MASK);
    }
    else
    {
        code_image[IC].source_reg = (uint8_t)(ZERO & MASK);
    }
    code_image[IC].target_address = (uint8_t)(address_method_des & MASK);

    // Handle target register if addressing is direct register
    if (address_method_des == 3)
    {

        int reg_des = strtol(&second_operand[1], NULL, DECIMAL);
        code_image[IC].target_reg = (uint8_t)(reg_des & MASK);
    }
    else
    {
        code_image[IC].target_reg = (uint8_t)(ZERO & MASK);
    }
    IC++; // Move to the next code word slot

    // No further operands to handle
    if (num_args == 0){
        return;
    }
        
    // Handle the first operand if it exists (only for 2 operands)
    if (num_args == 2)
    {
        switch (address_method_src)
        {
        case IMMEDIATE:
            first_operand += 1; // Skip '#'
            int num = strtol(first_operand, NULL, DECIMAL);
            code_u.all_bits = num;
            code_u.all_bits <<= THREE_BITS_SHIFT; // Shift 3 bits for A_R_E
            code_u.all_bits |= a_r_e.A;
            code_image[IC] = code_u.code_w;
            IC++;
            break;
            
        case DIRECT_REGISTER:
            // No need to allocate extra memory for register
            break;

        case RELATIVE:
        case DIRECT:
            // need to allocate place in code_image and set the data in second pass
            code_u.all_bits = ZERO;
            code_image[IC] = code_u.code_w;
            code_image[IC].first_operand=first_operand; 
            code_image[IC].second_operand=NULL;
            code_image[IC].source_address=address_method_src; //need to save the address method of src and target
            code_image[IC].target_address=address_method_des; //need to save the address method of src and target
            code_image[IC].place=1; // Mark this as first operand slot
            IC++;
            break;
        }
    }

    // for the scond operand of 2 args and first operand of 1 args
    switch (address_method_des)
    {
    case IMMEDIATE:
        second_operand += 1;  // Skip '#'
        int num = strtol(second_operand, NULL, DECIMAL);
        code_u.all_bits = num;
        code_u.all_bits <<= THREE_BITS_SHIFT;
        code_u.all_bits |= a_r_e.A;
        code_image[IC] = code_u.code_w;
        IC++;
        return;

    case DIRECT_REGISTER:
        // Register is handled in the first word, no additional space needed
        break;

    case RELATIVE:
    case DIRECT:
        // need to allocate place in code_image and set the data in second pass
        code_u.all_bits = ZERO;
        code_image[IC] = code_u.code_w;
        code_image[IC].first_operand=first_operand;
        code_image[IC].second_operand=second_operand;
        code_image[IC].source_address=address_method_src; //need to save the address method of src and target
        code_image[IC].target_address=address_method_des; //need to save the address method of src and target
        code_image[IC].place=TWO; // Mark this as second operand slot
        IC++;
        break;
    }
}
