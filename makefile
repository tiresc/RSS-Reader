CC=gcc
LIBS= -L. -lcurl
DEPS=parse.h

OBJ := rss.o

# Compiles each of the object files as separate targets
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(LIBS)

# -o $@ -> Tells make to put the output in a file named after the target
# $^ 	-> all of the file names in $(OBJ) separated by a space
rss: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

.PHONY: clean

# Target to clean files created during compilation
clean:
	rm -f *.o rss Makefile
	mv Makefile.orig Makefile