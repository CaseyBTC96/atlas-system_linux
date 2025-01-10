# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -g3

# Target executables
STRACE_TARGETS = strace_0 strace_1 strace_2 strace_3

# Rules for strace_0, strace_1, strace_2, and strace_3
strace_0: strace_0.c
	$(CC) $(CFLAGS) -o $@ $<

strace_1: strace_1.c
	$(CC) $(CFLAGS) -o $@ $<

strace_2: strace_2.c
	$(CC) $(CFLAGS) -o $@ $<

strace_3: strace_3.c
	$(CC) $(CFLAGS) -o $@ $<

# Rule to clean up all executables
clean:
	rm -f $(STRACE_TARGETS)

# Phony targets
.PHONY: clean $(STRACE_TARGETS)
