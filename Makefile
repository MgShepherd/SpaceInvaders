CC := clang
CFLAGS := -Wall -Wextra -I./include

C_FILES := main.c example.c
SRC_DIR := src
OUTPUT_DIR := build
TARGET := $(OUTPUT_DIR)/SpaceInvaders

OBJ_FILES := $(patsubst %.c, $(OUTPUT_DIR)/%.o, $(C_FILES))

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

$(OUTPUT_DIR)/%.o: $(SRC_DIR)/%.c | $(OUTPUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

clean:
	rm -rf $(OUTPUT_DIR)
