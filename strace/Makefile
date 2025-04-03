CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
CTAGS = ctags
TAGS = .tags
RM = rm -f
.PHONY: all clean distclean tags strace_0 strace_1 strace_2 strace_3 strace_4 strace_5 strace_6 strace_7 strace_8

all: strace_0 strace_1 strace_2 strace_3 strace_4 strace_5 strace_6 strace_7 strace_8

clean:
	$(RM) strace_0 strace_1 strace_2 strace_3 strace_4 strace_5 strace_6 strace_7 strace_8

distclean: clean
	$(RM) $(TAGS)

tags:
	$(CTAGS) -f $(TAGS) -R

strace_0: src/strace_0.c src/syscalls.c
	$(CC) $(CFLAGS) $^ -o $@

strace_1: src/strace_1.c src/syscalls.c
	$(CC) $(CFLAGS) $^ -o $@

strace_2: src/strace_2.c src/syscalls.c
	$(CC) $(CFLAGS) $^ -o $@

strace_3: src/strace_3.c src/syscalls.c
	$(CC) $(CFLAGS) $^ -o $@

strace_4: src/strace_4.c src/syscalls.c
	$(CC) $(CFLAGS) $^ -o $@

strace_5: src/strace_5.c src/syscalls.c
	$(CC) $(CFLAGS) $^ -o $@

strace_6: src/strace_6.c src/syscalls.c
	$(CC) $(CFLAGS) $^ -o $@

strace_7: src/strace_7.c src/syscalls.c
	$(CC) $(CFLAGS) $^ -o $@

strace_8: src/strace_8.c src/syscalls.c
	$(CC) $(CFLAGS) $^ -o $@
