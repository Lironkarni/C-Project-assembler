#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../headers/utils.h"
#include "../headers/error.h"
#include "../headers/globals.h"

void remove_extra_spaces(char *line)
{
    int j = 0;   // Index for writing cleaned characters
    int in_space = 0;    // Flag to track consecutive spaces
    int i = 0;    // Index for reading characters

    /* Skip leading spaces */
    while (line[i] == ' ')
    {
        i++;
    }

    /* Process the line character by character */
    for (; line[i] != '\0' && line[i] != '\n'; i++)
    {
        if (line[i] != ' ')
        {
            line[j++] = line[i]; // Copy non-space character
            in_space = 0;
        }
        else if (!in_space)
        {
            line[j++] = ' '; // Copy a single space
            in_space = 1;
        }
         // If already in space, skip additional spaces
    }
    /* Remove trailing space if present */
    if(line[j-1] == ' '){
        line[j-1] = '\0';
    }else{
        line[j] = '\0';
    }
}

int is_empty_line(const char *line)
{
    /* Iterate through each character in the line */
    while (*line)
    {
        /* If a non-whitespace character is found, the line is not empty */
        if (!isspace((unsigned char)*line))
        {
            return 0; // Line is not empty
        }
        line++;
    }
    return 1; // Line is empty or contains only spaces/tabs
}


int extraneous_text(char *command)
{
        int i;
    /* Iterate through the command string */
    for (i = 0; *command != NULL_CHAR; i++)
    {
        /* If a non-space, non-null character is found, it's extraneous text */
        if (command[i] != NULL_CHAR && command[i] != SPACE)
        {
            return 1;  /* Extraneous text found */
        }
    }

    /* If the string is empty or contains only spaces, return 0 */
    if (i == 0)
    {
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
        if (current[len] == COMMA)
        {
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