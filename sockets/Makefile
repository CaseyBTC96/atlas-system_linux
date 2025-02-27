# Compiler and flags
CC       = gcc
CFLAGS   = -g3 -Wall -Werror -Wextra -pedantic

# Output directories
BIN_DIR   = bin
OBJ_DIR   = obj

# Source files for todo_api
SRCS_TODO_API = 4-make_repsonse.c simple_server.c
OBJS_TODO_API = $(SRCS_TODO_API:%.c=$(OBJ_DIR)/%.o)

# Create output directories if they do not exist
$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR))

# Targets
all: $(BIN_DIR)/todo_api_0 $(BIN_DIR)/todo_api_1 $(BIN_DIR)/todo_api_2 $(BIN_DIR)/todo_api_3 $(BIN_DIR)/todo_api_4 $(BIN_DIR)/todo_api_5

$(BIN_DIR)/todo_api_0: $(OBJ_DIR)/4-make_response.o $(OBJ_DIR)/simple_server.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/todo_api_1: $(OBJ_DIR)/5-make_response.o $(OBJ_DIR)/simple_server.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/todo_api_2: $(OBJ_DIR)/6-make_response.o $(OBJ_DIR)/simple_server.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/todo_api_3: $(OBJ_DIR)/7-make_response.o $(OBJ_DIR)/simple_server.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/todo_api_4: $(OBJ_DIR)/8-make_response.o $(OBJ_DIR)/simple_server.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/todo_api_5: $(OBJ_DIR)/todo_api_5_files.o $(OBJ_DIR)/simple_server.o
	$(CC) $(CFLAGS) -o $@ $^

# Object file rules
$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*

.PHONY: all clean
