# compiler
CC = gcc
# select all .c files for src
src = $(wildcard *.c)
# associated object files
obj = $(src:.c=.o)
# build target executable
TARGET = smallsh

$(TARGET): $(obj)
	$(CC) -o $@ $^

# allow cleanup of executable and .o files with "make clean"
.PHONY: clean
clean:
	rm -f $(obj) $(TARGET)
