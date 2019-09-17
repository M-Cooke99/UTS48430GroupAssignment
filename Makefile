CC = gcc
CFLAGS = -Wall -Werror
LIBS = -lm
DEPS = timetable.h

#create .o file 
%.o: %.c $(DEPS)
	echo "Building $@"
	$(CC) $(CFLAGS) -c -o $@ $<

#create executable file
timetable: timetable.o
	echo "Building $@"
	$(CC) $(CFLAGS) -o timetable timetable.o $(LIBS) 

#prevent 'make' from doing something with a file named clean
.PHONY: clean

clean: 
	echo "Cleaning object files"
	rm -f ./*.o
