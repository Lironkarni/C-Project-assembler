#include "../headers/utils.h"

// הסרת רווחים מיותרים משורה
void remove_extra_spaces(char *line)
{
    int j = 0;
    int in_space = 0;
    int i = 0;

    while (line[i] == ' ')
    {
        i++;
    }

    for (; line[i] != '\0' && line[i] != '\n' ; i++) {
        if (line[i] != ' ') {
            line[j++] = line[i];
            in_space = 0;
        }
        else if (!in_space)
        {
            line[j++] = ' ';
            in_space = 1;
        }
    }
    if(line[j-1] == ' '){
        line[j-1] = '\0';
    }else{
        line[j] = '\0';
    }
    
}

int is_empty_line(const char *line)
{
    while (*line)
    {
        if (!isspace((unsigned char)*line))
        {
            return 0; // השורה אינה ריקה
        }
        line++;
    }
    return 1; // השורה ריקה או מכילה רק רווחים וטאבים
}


char *get_word(char *line)
{
	static char *current = NULL;
	// if its the first word in the line
	if (line != NULL)
	{
		current = line;
	}
	// got to the end of the row
	if (*current == NULL_CHAR)
	{
		return NULL;
	}
	char *word = current;
	// skip spaces
	while (*current == SPACE)
	{
		current++;
	}
	// size of the current word
	int len = 0;
	while (current[len] != NULL_CHAR && current[len] != SPACE)
	{
		len++;
		if (current[len] == COMMA){
			len++;
			break;
		}
	}
	// פסיקים
	if (current[len] == SPACE)  
	{
		int temp = len + 1;
		while (current[temp] == SPACE) 
			temp++;

		if (current[temp] == COMMA) 
		{
			len = temp + 1; 
		}
	}

	// allocate memory to the word
	word = (char *)malloc(len + 1);
	if (word == NULL)
	{
		print_system_error(ERROR_CODE_3);
		exit(1);
	}
	strncpy(word, current, len);
	word[len] = NULL_CHAR;

	current += len; // put current on next word
	if (*current == SPACE)
	{
		current++;
	}

	return word;
}

void print_bits(uint32_t value, int bits)

{
    for (int i = bits - 1; i >= 0; i--)
    {
        printf("%d", (value >> i) & 1);
    }
}



int extraneous_text(char *command)
{
int i;
    for(i=0;*command!=NULL_CHAR;i++)
    {
        if(command[i]!=NULL_CHAR && command[i]!=SPACE){
            return 1;
        }
    }
    if(i==0){
        return 0;
    }
    return 0;
}


void test(int dc, int ic)
{
    printf("%d\n", dc);
    printf("%d\n", ic);
    // הדפסת טבלת הסמלים
    Symbol *current = symbol_table_head;
    printf("Symbol Table:\n");
    printf("-----------------------------\n");
    while (current)
    {
        printf("Name: %s| Address: %d| Type: %d\n", current->name, current->address, current->type);
        current = current->next;
    }
    printf("-----------------------------\n\n");

    // הדפסת תמונת הנתונים
    printf("Data Image:\n");
    printf("-----------------------------\n");
    for (int i = 0; i < dc; i++)
    {
        printf("Address %d: %06X\n", i+ic, data_image[i].data);
    }
    printf("-----------------------------\n");

    printf("Code Image\n");
    printf("%d\n", ic);
    // הדפסת תמונת הקוד
    for (int i = 100; i < ic; i++)
    {
        // בניית 24 ביטים מהמבנה
        // uint32_t full_word = (code_image[i].op_code << 18) |
        //                      (code_image[i].source_address << 16) |
        //                      (code_image[i].source_reg << 13) |
        //                      (code_image[i].target_address << 11) |
        //                      (code_image[i].target_reg << 8) |
        //                      (code_image[i].funct << 3) |
        //                      (code_image[i].A_R_E);
        uint32_t full_word = 0;
        memcpy(&full_word, &code_image[i], 3); // מעתיקים רק 3 בתים (24 ביטים)

        printf("Address %d: ", i);
        print_bits(full_word, 24); // הדפסת כל הביטים
        printf(" (0x%06X)\n", full_word);
    }
    printf("-----------------------------\n");

    // ext_ent_list *current_ext = ext_ent_list_head;
    // printf("extern entry Table:\n");
    // printf("-----------------------------\n");
    // while (current_ext)
    // {
    //     printf("Name: %s| Type: %d| line_number: %d\n", current_ext->label_name, current_ext->type, current_ext->line->line_number);
    //     current_ext = current_ext->next;
    // }
    // printf("-----------------------------\n\n");
}
