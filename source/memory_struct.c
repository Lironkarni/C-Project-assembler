#include "../headers/memory_struct.h"

int IC = 100;
int DC = 0;

code_word code_image[MEM_SIZE];
data_word data_image[MEM_SIZE];

A_R_E a_r_e = {4, 2, 1};

void add_data(data_word *data_image, int *numbers, Line *line)
{
    int numbers_size = 0;
    // int len = sizeof(numbers) / sizeof(numbers[0]);
    while (numbers[numbers_size] != '\0')
    {
        numbers_size++;
    }

    if (DC + numbers_size + 1 > MEM_SIZE)
    {
        // memory overflow
        print_system_error(ERROR_CODE_3);
        return;
    }

    for (int i = 0; i < numbers_size; i++)
    {
        data_image[DC].data = (uint32_t)(numbers[i] & 0xFFFFFF);
        DC++;
    }
}

/*this function get the data if its ".data", and check if its valid or there are errors*/
int get_data(Line *line, int inst_index, int **numbers)
{
    int expect_number = 1; // 0- expect number, 1- expect comma
    int capacity, count = 0;
    char *data_ptr = strstr(line->data, instruction_list[inst_index]);
    data_ptr += strlen(instruction_list[inst_index]);
    while (*data_ptr == ' ')
        data_ptr++;

    capacity = INIT_CAPACITY;
    *numbers = (int *)malloc(capacity * sizeof(int));
    if (*numbers == NULL)
    {
        print_system_error(ERROR_CODE_3);
        exit(1);
    }
    while (*data_ptr != NULL_CHAR)
    {
        if (expect_number)
        {
            char *end_ptr;
            if (*data_ptr == MINUS || *data_ptr == PLUS)
            {
                if (!isdigit(*(data_ptr + 1)))
                { // אם אחרי '-' או '+' לא בא מספר - שגיאה
                    print_syntax_error(22, line->file_name, line->line_number);
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

            int num=strtol(data_ptr, &end_ptr, DECIMAL);
            if (*end_ptr == '.') { 
                print_syntax_error(ERROR_CODE_41, line->file_name, line->line_number);
                free(*numbers);
                return 1;
            }
            if (count >= capacity)
            {
                capacity *= 2;
                *numbers = (int *)realloc(*numbers, capacity * sizeof(int));
                if (*numbers == NULL)
                {
                    print_system_error(ERROR_CODE_3);
                    exit(1);
                }
            }
            (*numbers)[count++] = num;
            expect_number = 0; // expecting now a comma
            data_ptr= end_ptr;
        }
        else
        {
            if (*data_ptr != COMMA)
            {
                print_syntax_error(ERROR_CODE_23, line->file_name, line->line_number);
                free(*numbers);
                return 1;
            }
            expect_number = 1;
            data_ptr++;
        }
        while (*data_ptr == SPACE)
            data_ptr++;
    }
    // check if line ended with comma
    if (expect_number)
    {
        print_syntax_error(ERROR_CODE_24, line->file_name, line->line_number);
        free(*numbers);
        return 1;
    }
    (*numbers)[count] = NULL_CHAR;
    return 0;
}

int get_string_data(Line *line, int inst_index, char **characters)
{
    int capacity = INIT_CAPACITY, length = 0;
    char *char_array;
    char *data_ptr = strstr(line->data, instruction_list[inst_index]);

    if (!data_ptr)
    {
        print_syntax_error(ERROR_CODE_22, line->file_name, line->line_number);
        return 1;
    }

    data_ptr += strlen(instruction_list[inst_index]);

    while (*data_ptr == SPACE) // דילוג על רווחים
        data_ptr++;

    if (*data_ptr != QUOTE)
    { // אם המחרוזת לא מתחילה בגרשיים - שגיאה
        print_syntax_error(ERROR_CODE_20, line->file_name, line->line_number);
        return 1;
    }

    data_ptr++; // מעבר אחרי המרכאה הפותחת

    char_array = (char *)malloc(capacity * sizeof(char));
    if (!char_array)
    {
        print_system_error(ERROR_CODE_3);
        exit(1);
    }

    // קריאת המחרוזת עד המרכאה הסוגרת
    while (*data_ptr && *data_ptr != QUOTE)
    {
        if (length >= capacity)
        {
            capacity *= 2;
            char_array = (char *)realloc(char_array, capacity * sizeof(char));
            if (!char_array)
            {
                print_system_error(ERROR_CODE_3);
                exit(1);
            }
        }
        char_array[length++] = *data_ptr++;
    }

    if (*data_ptr != QUOTE)
    { // אם אין מרכאה סוגרת - שגיאה
        print_syntax_error(ERROR_CODE_20, line->file_name, line->line_number);
        free(char_array);
        return 1;
    }

    char_array[length] = NULL_CHAR; // סימון סוף מחרוזת
    *characters = char_array;

    data_ptr++; // מעבר אחרי המרכאה הסוגרת

    // דילוג על רווחים אחרי המחרוזת
    while (*data_ptr == SPACE)
        data_ptr++;

    // אם יש עוד תווים אחרי המחרוזת - שגיאה
    if (*data_ptr != NULL_CHAR)
    {
        print_syntax_error(ERROR_CODE_21, line->file_name, line->line_number);
        free(char_array);
        return 1;
    }

    return 0; // הצלחה
}

void add_string_data(data_word *data_image, char *char_array, Line *line)
{
    int length = strlen(char_array);

    if (DC + length + 1 > MEM_SIZE)
    {
        print_system_error(ERROR_CODE_3);
        return;
    }

    for (int i = 0; i <= length; i++)
    {
        if (i == length)
        {
            data_image[DC].data = 0;
        }
        else
        {
            data_image[DC].data = (uint32_t)(char_array[i] & 0xFF); // רק ה-8 ביט התחתונים
        }
        DC++;
    }
}

void add_to_code_image(code_word *code_image, Line *line, int num_args, int op_index, int funct, int address_method_src, int address_method_des, char *first_operand, char *second_operand)
{
    if(IC>=MEM_SIZE)
    {
        print_system_error(ERROR_CODE_37);
        //???
        // stop the program? stop forst pass?
    }
    code_union code_u;
    code_image[IC].op_code = (uint8_t)(op_index & MASK);
    code_image[IC].A_R_E = (uint8_t)(a_r_e.A & MASK);
    code_image[IC].funct = (uint8_t)(funct & MASK);
    code_image[IC].source_address = (uint8_t)(address_method_src & MASK);
    if (address_method_src == 3)
    {

        int reg_src = strtol(&first_operand[1], NULL, DECIMAL);
        code_image[IC].source_reg = (uint8_t)(reg_src & MASK);
    }
    else
    {
        code_image[IC].source_reg = (uint8_t)(ZERO & MASK);
    }
    code_image[IC].target_address = (uint8_t)(address_method_des & MASK);
    if (address_method_des == 3)
    {

        int reg_des = strtol(&second_operand[1], NULL, DECIMAL);
        code_image[IC].target_reg = (uint8_t)(reg_des & MASK);
    }
    else
    {
        code_image[IC].target_reg = (uint8_t)(ZERO & MASK);
    }
    IC++;
    if (num_args == 0){
        return;
    }
        
    // מה שיטת המיעון של האופרנד
    // מיידי- זה מספר
    // ישיר או יחסי- לעשות הקצאה למקום
    // חוץ מרגיסטר כולם צריכים הקצאה של עוד שורה בטבלת הקוד

    if (num_args == 2)
    {
        switch (address_method_src)
        {
        case IMMEDIATE:
            first_operand += 1;
            int num = strtol(first_operand, NULL, DECIMAL);
            code_u.all_bits = num;
            code_u.all_bits <<= THREE_BITS_SHIFT;
            code_u.all_bits |= a_r_e.A;

            code_image[IC] = code_u.code_w;
            IC++;

            break;
            ;
        case DIRECT_REGISTER:
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

            code_image[IC].place=1;
            IC++;
            break;
        }
    }
    // for the scond operand of 2 args and first operand of 1 args
    switch (address_method_des)
    {
    case IMMEDIATE:
        second_operand += 1;
        int num = strtol(second_operand, NULL, DECIMAL);
        code_u.all_bits = num;
        code_u.all_bits <<= THREE_BITS_SHIFT;
        code_u.all_bits |= a_r_e.A;

        code_image[IC] = code_u.code_w;
        IC++;
        return;
    case DIRECT_REGISTER:
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
        code_image[IC].place=TWO;

        IC++;
        break;
    }
}
