# Makefile עבור הפרויקט assembler

# שם הקובץ הסופי
TARGET = assembler

# הקומפיילר
CC = gcc

# דגלי קומפילציה (כולל את הנתיב של קבצי ההדרים)
CFLAGS = -Wall -ansi -pedantic -std=c11 -g -Iinclude 

# כל קבצי המקור (source files)
SRC = source/main.c source/utils.c source/error.c source/first_pass.c source/process_input.c source/memory_struct.c source/label.c source/second_pass.c

# כל קבצי האובייקט (object files) שנוצרים מקבצי המקור
OBJ = $(SRC:.c=.o)

# כלל ברירת מחדל
all: $(TARGET)

# כלל ליצירת הקובץ הסופי
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# כלל ליצירת קבצי אובייקט
source/%.o: source/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# כלל לניקוי קבצים
clean:
	del /F /Q source\*.o assembler 2>nul || exit 0

