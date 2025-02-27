# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Werror -Wextra -pedantic -std=gnu89 -g

# Phony targets
.PHONY: all todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 todo_api_5 clean

# All targets
all: todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 todo_api_5

# Individual targets
todo_api_0:
	$(CC) $(CFLAGS) 0-todo_api.c -o todo_api_0

todo_api_1:
	$(CC) $(CFLAGS) 1-todo_api.c -o todo_api_1

todo_api_2:
	$(CC) $(CFLAGS) 2-todo_api.c -o todo_api_2

todo_api_3:
	$(CC) $(CFLAGS) 3-todo_api.c -o todo_api_3

todo_api_4:
	$(CC) $(CFLAGS) 4-todo_api.c -o todo_api_4

todo_api_5:
	$(CC) $(CFLAGS) 5-todo_api.c -o todo_api_5

# Clean up build artifacts
clean:
	rm -f todo_api_*
