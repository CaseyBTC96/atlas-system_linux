# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic

# Directories
SRC_DIR = src
BIN_DIR = bin

# Source files for different todo_api versions
TODO_API_0_SRC = $(SRC_DIR)/tcp_server.c
TODO_API_1_SRC = $(SRC_DIR)/tcp_client.c
TODO_API_2_SRC = $(SRC_DIR)/another_api.c  # Change as needed
TODO_API_3_SRC = $(SRC_DIR)/yet_another_api.c
TODO_API_4_SRC = $(SRC_DIR)/more_api.c
TODO_API_5_SRC = $(SRC_DIR)/final_api.c

# Output binaries for different todo_api versions
TODO_API_0_BIN = $(BIN_DIR)/todo_api_0
TODO_API_1_BIN = $(BIN_DIR)/todo_api_1
TODO_API_2_BIN = $(BIN_DIR)/todo_api_2
TODO_API_3_BIN = $(BIN_DIR)/todo_api_3
TODO_API_4_BIN = $(BIN_DIR)/todo_api_4
TODO_API_5_BIN = $(BIN_DIR)/todo_api_5

# Default target
all: $(TODO_API_0_BIN) $(TODO_API_1_BIN) $(TODO_API_2_BIN) $(TODO_API_3_BIN) $(TODO_API_4_BIN) $(TODO_API_5_BIN)

# Individual targets
todo_api_0: $(TODO_API_0_BIN)
	@echo "Built todo_api_0."

todo_api_1: $(TODO_API_1_BIN)
	@echo "Built todo_api_1."

todo_api_2: $(TODO_API_2_BIN)
	@echo "Built todo_api_2."

todo_api_3: $(TODO_API_3_BIN)
	@echo "Built todo_api_3."

todo_api_4: $(TODO_API_4_BIN)
	@echo "Built todo_api_4."

todo_api_5: $(TODO_API_5_BIN)
	@echo "Built todo_api_5."

# Rules to build binaries
$(TODO_API_0_BIN): $(TODO_API_0_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(TODO_API_1_BIN): $(TODO_API_1_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(TODO_API_2_BIN): $(TODO_API_2_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(TODO_API_3_BIN): $(TODO_API_3_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(TODO_API_4_BIN): $(TODO_API_4_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(TODO_API_5_BIN): $(TODO_API_5_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

# Clean up built binaries
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 todo_api_5
