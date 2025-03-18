#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../headers/utils.h"
#include "../headers/error.h"
#include "../headers/process_input.h"
#include "../headers/globals.h"


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


// טעינת קובץ עם הקצאה דינאמית
char *load_file(char *filename) {

    char *filename_with_extension = (char *)malloc(strlen("test-files/") + strlen(filename) + strlen(".as") + 1);
    if (!filename_with_extension) {
        print_system_error(ERROR_CODE_3);
        return NULL;
    }
        sprintf(filename_with_extension, "test-files/%s.as", filename);

    FILE *file = fopen(filename_with_extension, "r");
    if (!file)
    {
        print_system_error(ERROR_CODE_2);
        free(filename_with_extension);
        return NULL;
    }
    fclose(file);
    return filename_with_extension;
    }

void delete_am_file(const char *filename) {

    size_t len = strlen(filename) + strlen("test-files/") + strlen(".am") + 1;

    char *am_filename = (char *)malloc(len);
    if (am_filename == NULL) {
        print_system_error(ERROR_CODE_3);
        return;
    }

    snprintf(am_filename, len, "test-files/%s.am", filename);

    if (remove(am_filename) == 0) {
        printf("File %s deleted successfully.\n", am_filename);
    } else {
        print_system_error(ERROR_CODE_7);
    }

    free(am_filename);
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