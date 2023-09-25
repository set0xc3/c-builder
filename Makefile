BUILD_DIR 		:= build
BUILD_BIN_DIR := $(BUILD_DIR)/bin
BUILD_LIB_DIR := $(BUILD_DIR)/lib

SRC_DIR 		:= src
TESTS_DIR 	:= tests
DIR 	:= vendor

MICROUI_DIR := $(DIR)/microui/src

GLAD_INC_DIR := $(DIR)/glad/include
GLAD_SRC_DIR := $(DIR)/glad/src

INC_DIRS 	:= -I$(SRC_DIR) -I$(DIR)/microui/src -I$(GLAD_INC_DIR)
INC_FLAGS := -lm -lGL -lvulkan -lSDL2
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
MICROUI_SRCS	:= $(shell find $(MICROUI_DIR) -type f -name '*.c')
TESTS_SRCS		:= $(shell find $(TESTS_DIR) -type f -name '*.c')

GLAD_SRCS := $(shell find $(GLAD_SRC_DIR) -type f -name '*.c')

OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/$(SRC_DIR)/%.o, $(SRCS)) \
        $(patsubst $(MICROUI_DIR)/%.c, $(BUILD_DIR)/$(MICROUI_DIR)/%.o, $(MICROUI_SRCS))

GLAD_OBJS := $(patsubst $(GLAD_SRC_DIR)/%.c, $(BUILD_DIR)/$(GLAD_SRC_DIR)/%.o, $(GLAD_SRCS))

TESTS_OBJS := $(OBJS) \
							$(GLAD_OBJS) \
							$(patsubst $(TESTS_DIR)/%.c, $(BUILD_DIR)/$(TESTS_DIR)/%.o, $(TESTS_SRCS))

.DEFAULT_GOAL := all

all: pre-build build-all

$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(MICROUI_DIR)/%.o: $(MICROUI_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TESTS_DIR)/%.o: $(TESTS_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(GLAD_SRC_DIR)/%.o: $(GLAD_SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_BIN_DIR)/%.bin: $(BUILD_DIR)/$(TESTS_DIR)/%.o $(TESTS_OBJS)
	mkdir -p $(BUILD_BIN_DIR)
	$(CC) $(CFLAGS) $(INC_FLAGS) $(GLAD_OBJS) $(OBJS) $< -o $@

pre-build:
	mkdir -p $(BUILD_DIR)/bin
	ln -fsr assets/ $(BUILD_BIN_DIR)

build-tests: \
	$(patsubst $(BUILD_DIR)/$(TESTS_DIR)/%.o, $(BUILD_BIN_DIR)/%.bin, $(GLAD_OBJS) $(TESTS_OBJS))

build-all: \
	build-tests

build-shaders:
	mkdir -p $(BUILD_DIR)/assets/shaders
	/usr/bin/glslc assets/shaders/simple.vert -o $(BUILD_DIR)/assets/shaders/simple.vert.spv
	/usr/bin/glslc assets/shaders/simple.frag -o $(BUILD_DIR)/assets/shaders/simple.frag.spv

test-opengl:
	@$(BUILD_BIN_DIR)/test_opengl.bin

test-os:
	@$(BUILD_BIN_DIR)/test_os.bin

test-vulkan:
	@$(BUILD_BIN_DIR)/test_vulkan.bin

tests: \
	test-opengl \
	test-os \
	test-vulkan

clean:
	rm -f compile_commands.json
	rm -rf .cache
	rm -rf $(BUILD_DIR)
