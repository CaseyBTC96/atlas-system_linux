# Define the compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -g

# Define the source files and targets
SRC = $(wildcard *.c)
TARGETS = strace_0 strace_1 strace_2 strace_3

# Pattern for object files
OBJ = $(SRC:.c=.o)

# Default rule to build all targets
all: $(TARGETS)

# Rule to build each target
$(TARGETS): %: %.o
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean the build
clean:
	rm -f $(TARGETS) *.o

# Rule to check for memory leaks using Valgrind
valgrind: $(TARGETS)
	valgrind ./strace_0 /bin/echo hello
	valgrind ./strace_1 /bin/echo hello
	valgrind ./strace_2 /bin/echo hello
	valgrind ./strace_3 /bin/echo hello

.PHONY: all clean valgrind
