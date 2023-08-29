SANDBOX_TARGET	:= sandbox.bin
TESTS_TARGET 		:= tests.bin

BUILD_DIR 		:= ./build
BUILD_BIN_DIR := $(BUILD_DIR)/bin
BUILD_LIB_DIR := $(BUILD_DIR)/lib

SRC_DIR 		:= ./src
TESTS_DIR 	:= ./tests
SANDBOX_DIR := ./sandbox
VENDOR_DIR 	:= ./vendor
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

SANDBOX_OBJS := $(OBJS)
SANDBOX_OBJS += $(patsubst $(SANDBOX_DIR)/%.c, $(BUILD_DIR)/$(SANDBOX_DIR)/%.o, $(SANDBOX_SRCS))

.DEFAULT_GOAL := all

$(BUILD_DIR)/$(SRC_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(SANDBOX_DIR)/%.o : $(SANDBOX_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TESTS_DIR)/%.o : $(TESTS_DIR)/%.c	
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(MICROUI_DIR)/%.o : $(MICROUI_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_BIN_DIR)/$(SANDBOX_TARGET) : $(SANDBOX_OBJS)
	mkdir -p $(BUILD_BIN_DIR)
	$(CC) $(CFLAGS) $(SANDBOX_OBJS) -o $@ $(INC_FLAGS)

$(BUILD_BIN_DIR)/$(TESTS_TARGET) : $(TESTS_OBJS)
	mkdir -p $(BUILD_BIN_DIR)
	$(CC) $(CFLAGS) $(TESTS_OBJS) -o $@ $(INC_FLAGS)

all : build-sandbox build-test

build-sandbox : $(BUILD_BIN_DIR)/$(SANDBOX_TARGET)

build-test : $(BUILD_BIN_DIR)/$(TESTS_TARGET)

run-sandbox :
	$(BUILD_BIN_DIR)/$(SANDBOX_TARGET)

run-test :
	$(BUILD_BIN_DIR)/$(TESTS_TARGET)

pre-build :
	mkdir -p build/assets/shaders
	/usr/bin/glslc assets/shaders/simple.vert -o build/assets/shaders/simple.vert.spv
	/usr/bin/glslc assets/shaders/simple.frag -o build/assets/shaders/simple.frag.spv

clean :
	rm -f compile_commands.json
	rm -rf .cache
	rm -rf $(BUILD_DIR)
