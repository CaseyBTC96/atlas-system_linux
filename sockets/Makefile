# Compiler and flags
CC       = gcc
CFLAGS   = -g3 -Wall -Werror -Wextra -pedantic

# Output directories
BIN_DIR   = bin
OBJ_DIR   = obj

# Source files
SRCS_SERVER = 0-server.c 1-server.c 3-server.c
SRCS_CLIENT = 2-client.c

# Object files
OBJS_SERVER = $(SRCS_SERVER:%.c=$(OBJ_DIR)/%.o)
OBJS_CLIENT = $(SRCS_CLIENT:%.c=$(OBJ_DIR)/%.o)

# Create output directories if they do not exist
$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR))

# Targets
all: $(BIN_DIR)/0-server $(BIN_DIR)/1-server $(BIN_DIR)/2-client $(BIN_DIR)/3-server

$(BIN_DIR)/0-server: $(OBJS_SERVER)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/1-server: $(OBJ_DIR)/1-server.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/2-client: $(OBJ_DIR)/2-client.o
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/3-server: $(OBJ_DIR)/3-server.o
	$(CC) $(CFLAGS) -o $@ $^

# Object file rules
$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*

.PHONY: all clean
