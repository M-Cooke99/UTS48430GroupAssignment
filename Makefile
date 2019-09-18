CC = gcc
CFLAGS = -Wall -Werror
LIBS = -lm
DEPS = huffman.h

#create .o file 
%.o: %.c $(DEPS)
	echo "Building $@"
	$(CC) $(CFLAGS) -c -o $@ $<

#create executable file
huffman: huffman.o
	echo "Building $@"
	$(CC) $(CFLAGS) -o huffman huffman.o $(LIBS) 

#prevent 'make' from doing something with a file named clean
.PHONY: clean

clean: 
	echo "Cleaning object files"
	rm -f ./*.o
