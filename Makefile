TARGET := sandbox.bin

BUILD_DIR := ./build
SRC_DIR := ./src
SANDBOX_DIR := ./sandbox
VENDOR_DIR := ./vendor
MICROUI_DIR := $(VENDOR_DIR)/microui/src

CC := clang
INC_DIRS := -I$(SRC_DIR) -I$(SANDBOX_DIR) -I$(VENDOR_DIR)/microui/src
INC_FLAGS := -lm -lGL -lSDL2
OPT := -O0

NOWARN := -Wno-gnu-zero-variadic-macro-arguments \
				 -Wno-unused-parameter \
				 -Wno-unused-variable \
				 -Wno-unused-but-set-variable \
				 -Wno-macro-redefined \
				 -Wno-attributes \
				 -Wno-incompatible-library-redeclaration

CFLAGS := $(INC_DIRS) $(OPT) -std=c11 -g -Wall -Wextra -pedantic $(NOWARN)

SRCS := $(shell find $(SRC_DIR) -type f -name '*.c')
SANDBOX_SRCS := $(shell find $(SANDBOX_DIR) -type f -name '*.c')
MICROUI_SRCS := $(shell find $(MICROUI_DIR) -type f -name '*.c')

OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS)) \
        $(patsubst $(SANDBOX_DIR)/%.c, $(BUILD_DIR)/%.o, $(SANDBOX_SRCS)) \
        $(patsubst $(MICROUI_DIR)/%.c, $(BUILD_DIR)/%.o, $(MICROUI_SRCS))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SANDBOX_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(MICROUI_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(INC_FLAGS)

run :
	$(BUILD_DIR)/$(TARGET)

clean :
	rm -f compile_commands.json
	rm -rf .cache
	rm -rf $(BUILD_DIR)
