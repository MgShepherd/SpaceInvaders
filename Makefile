CC := clang

# TODO Make these library paths configurable by the user
LIB_DIR := /opt/homebrew
LIBS := raylib

CFLAGS := -Wall -Wextra -I./include -I$(LIB_DIR)/include -L$(LIB_DIR)/lib -l$(LIBS)

C_FILES := main.c game.c
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
