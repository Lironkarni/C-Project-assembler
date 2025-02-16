#include "../headers/memory_struct.h"
#include "../headers/first_pass.h"

int IC = 100;
int DC = 0;

A_R_E a_r_e={4,2,1};

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

    for (int i = 0; i <= numbers_size; i++)
    {
        if (i == numbers_size)
        {
            data_image[DC].data = 0;
        }
        else
        {
            data_image[DC].data = (uint32_t)(numbers[i] & 0xFFFFFF);
        }
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
            data_image[DC].data = (uint32_t)(char_array[i] & 0xFF); // רק ה-8 ביט התחתונים
        }
        DC++;
    }
}

void add_to_code_image(code_word *code_image, Line *line, int num_args, int op_index)
{
    // A_R_E A = 4;

    if (num_args == 0)
    {

        code_image[IC].op_code = (uint8_t)(op_index & MASK);
        code_image[IC].A_R_E = (uint8_t)(a_r_e.A & MASK);
        code_image[IC].funct = (uint8_t)(ZERO & MASK);
        code_image[IC].source_address = (uint8_t)(ZERO & MASK);
        code_image[IC].source_reg = (uint8_t)(ZERO & MASK);
        code_image[IC].target_address = (uint8_t)(ZERO & MASK);
        code_image[IC].target_reg = (uint8_t)(ZERO & MASK);
        IC++;
    }
}

