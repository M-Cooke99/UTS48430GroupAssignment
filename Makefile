CC = gcc
CFLAGS = -Wall -Werror
LIBS = -lm
DEPS = huffman.h
SRC = huffman.c
OBJ = $(SRC:.c=.o)
EXE = huffman
RM = rm -f

#create .o file 
%.o: %.c $(DEPS)
	echo "Building $@"
	$(CC) $(CFLAGS) -c -o $@ $<

#create executable file
$(EXE) : $(OBJ)
	echo "Building $@"
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) $(LIBS) 

#prevent 'make' from doing something with a file named clean
.PHONY: clean

clean: 
	echo "Cleaning object files"
	$(RM) $(EXE) ./*.o
