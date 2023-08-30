SANDBOX_TARGET := sandbox.bin

BUILD_DIR 		:= build
BUILD_BIN_DIR := $(BUILD_DIR)/bin
BUILD_LIB_DIR := $(BUILD_DIR)/lib

SRC_DIR 		:= src
TESTS_DIR 	:= tests
SANDBOX_DIR := sandbox
VENDOR_DIR 	:= vendor
MICROUI_DIR := $(VENDOR_DIR)/microui/src

CC 				:= clang
INC_DIRS 	:= -I$(SRC_DIR) -I$(SANDBOX_DIR) -I$(VENDOR_DIR)/microui/src
INC_FLAGS := -lm -lvulkan -lSDL2
OPT 			:= -O0

NOWARN := -Wno-gnu-zero-variadic-macro-arguments \
				 -Wno-unused-parameter \
				 -Wno-unused-variable \
				 -Wno-unused-but-set-variable \
				 -Wno-macro-redefined \
				 -Wno-attributes \
				 -Wno-incompatible-library-redeclaration

CFLAGS := $(INC_DIRS) $(OPT) -std=c11 -g -Wall -Wextra -pedantic $(NOWARN)

SRCS 					:= $(shell find $(SRC_DIR) -type f -name '*.c')
SANDBOX_SRCS	:= $(shell find $(SANDBOX_DIR) -type f -name '*.c')
MICROUI_SRCS	:= $(shell find $(MICROUI_DIR) -type f -name '*.c')
TESTS_SRCS		:= $(shell find $(TESTS_DIR) -type f -name '*.c')

OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/$(SRC_DIR)/%.o, $(SRCS)) \
        $(patsubst $(MICROUI_DIR)/%.c, $(BUILD_DIR)/$(MICROUI_DIR)/%.o, $(MICROUI_SRCS))

TESTS_OBJS := $(OBJS)
TESTS_OBJS += $(patsubst $(TESTS_DIR)/%.c, $(BUILD_DIR)/$(TESTS_DIR)/%.o, $(TESTS_SRCS))

TESTS_BINS := $(patsubst $(BUILD_DIR)/$(TESTS_DIR)/%.o, $(BUILD_BIN_DIR)/%.bin, $(TESTS_OBJS))

SANDBOX_OBJS := $(OBJS)
SANDBOX_OBJS += $(patsubst $(SANDBOX_DIR)/%.c, $(BUILD_DIR)/$(SANDBOX_DIR)/%.o, $(SANDBOX_SRCS))

.DEFAULT_GOAL := all

all: pre-build build-sandbox build-tests

$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(SANDBOX_DIR)/%.o: $(SANDBOX_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(MICROUI_DIR)/%.o: $(MICROUI_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TESTS_DIR)/%.o: $(TESTS_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_BIN_DIR)/$(SANDBOX_TARGET): $(SANDBOX_OBJS)
	mkdir -p $(BUILD_BIN_DIR)
	$(CC) $(CFLAGS) $(SANDBOX_OBJS) -o $@ $(INC_FLAGS)

$(BUILD_BIN_DIR)/%.bin: $(BUILD_DIR)/$(TESTS_DIR)/%.o $(TESTS_OBJS)
	mkdir -p $(BUILD_BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) $< -o $@ $(INC_FLAGS)

pre-build:
	mkdir -p $(BUILD_DIR)/bin
	ln -fsr assets/ $(BUILD_BIN_DIR)

build-sandbox: $(BUILD_BIN_DIR)/$(SANDBOX_TARGET)

build-tests: $(TESTS_BINS)

build-shaders:
	mkdir -p $(BUILD_DIR)/assets/shaders
	/usr/bin/glslc assets/shaders/simple.vert -o $(BUILD_DIR)/assets/shaders/simple.vert.spv
	/usr/bin/glslc assets/shaders/simple.frag -o $(BUILD_DIR)/assets/shaders/simple.frag.spv

run-sandbox: 
	@$(BUILD_BIN_DIR)/$(SANDBOX_TARGET)

test-os:
	@$(BUILD_BIN_DIR)/test_os.bin

test-vulkan:
	@$(BUILD_BIN_DIR)/test_vulkan.bin

tests: \
	test-os \
	test-vulkan

clean:
	rm -f compile_commands.json
	rm -rf .cache
	rm -rf $(BUILD_DIR)
