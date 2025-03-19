#include "../headers/line.h"
#include "../headers/error.h"

Line *create_line(char* temp_line,char* file,int line_number){
    
    Line *new_line;

    /* Allocate memory for the new Line struct */
    new_line=(Line *)malloc(sizeof(Line));
    if(new_line==NULL){
        print_system_error(ERROR_CODE_3); // Memory allocation failed
        return NULL;
    }
    
    /* Allocate memory for the file name and copy it */
    new_line->file_name=(char *)malloc(strlen(file)+1);
    if(new_line->file_name==NULL){
        print_system_error(ERROR_CODE_3);
       return NULL;
    }
    
    strcpy(new_line->file_name,file);
    
    /* Allocate memory for the line data and copy it */
    new_line->data=(char *)malloc(strlen(temp_line)+1);
    if(new_line->data==NULL){
        print_system_error(ERROR_CODE_3);
       return NULL;
    }
    
    strcpy(new_line->data,temp_line);

    /* Store the line number */
    new_line->line_number=line_number;
    
    return new_line;  // Return the created Line struct
}
