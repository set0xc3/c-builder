TARGET = sandbox.bin

CC = clang
INCDIRS = -I. -Isrc -Isandbox
CLIBS = -lm -lSDL2
OPT = -O0

CFLAGS = $(INCDIRS) $(CLIBS) $(OPT) -MJ compile_commands.json -g -Wall -Wextra -std=c99

PREF_BUILD = ./build/

SRC = $(wildcard ./sandbox/*.c) $(wildcard ./src/**/*.c)

all : $(TARGET)

$(TARGET) : $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(PREF_BUILD)$(TARGET)

run :
	$(PREF_BUILD)$(TARGET)

clean :
	rm -f compile_commands.json
	rm -f $(PREF_BUILD)$(TARGET) $(PREF_OBJ)*.o
