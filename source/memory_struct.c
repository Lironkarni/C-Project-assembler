#include "../headers/memory_struct.h"
#include "../headers/first_pass.h"
#include "../headers/process_input.h"

int IC = 100;
int DC = 0;

A_R_E a_r_e = {4, 2, 1};

void add_data(data_word *data_image, int *numbers, Line *line)
{
    int numbers_size = 0;
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
            data_image[DC].data = (uint32_t)(char_array[i] & 0xFF); // The lowest 8 bits only
        }
        DC++;
    }
}

void add_to_code_image(code_word *code_image, Line *line, int num_args, int op_index, int funct, int address_method_src, int address_method_des, char *first_operand, char *second_operand)
{
    if(IC>=MEM_SIZE)
    {
        print_system_error(ERROR_CODE_37);
        
        // TODO: stop the program? stop forst pass?
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
