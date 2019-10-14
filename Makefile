CC = gcc
CFLAGS = -Wall -Werror
LIBS = -lm
DEPS = timetable.
SRC = timetable.c
OBJ = $(SRC:.c=.o)
EXE = timetable
RM = rm -f 

#create .o file 
%.o: %.c $(DEPS)
	echo "Building $@"
	$(CC) $(CFLAGS) -c -o $@ $<

#create executable file
$(EXE): $(OBJ)
	echo "Building $@"
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) $(LIBS) 

#prevent 'make' from doing something with a file named clean
.PHONY: clean

clean: 
	echo "Cleaning execute and object files"
	$(RM) $(EXE) ./*.o
