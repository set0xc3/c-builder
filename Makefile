TARGET = sandbox.bin

CC = clang
INCDIRS = -I. -Isrc -Isandbox -Ivendor/microui/src
CLIBS = -lm -lGL -lSDL2
OPT = -O0

NOWARN = -Wno-gnu-zero-variadic-macro-arguments \
				 -Wno-unused-parameter \
				 -Wno-unused-variable \
				 -Wno-unused-but-set-variable \
				 -Wno-macro-redefined \
				 -Wno-attributes \
				 -Wno-incompatible-library-redeclaration

CFLAGS = $(INCDIRS) $(CLIBS) $(OPT) -g -Wall -Wextra -pedantic -std=c11 $(NOWARN)

PREF_BUILD = ./build/

SRC = $(wildcard ./src/**/*.c) \
			$(wildcard ./sandbox/*.c) \
			$(wildcard ./vendor/microui/src/*.c)

all : $(TARGET)

$(TARGET) : $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(PREF_BUILD)$(TARGET)

run :
	$(PREF_BUILD)$(TARGET)

clean :
	rm -rf .cache
	rm -f compile_commands.json
	rm -f $(PREF_BUILD)$(TARGET) $(PREF_OBJ)*.o
