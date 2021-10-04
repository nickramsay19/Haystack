# tool macros
CC := gcc
FLAGS := 

# path macros
PATHS = $(SRC_PATH) $(OBJ_PATH) $(BIN_PATH) $(TESTS_PATH) $(INCLUDE_PATH)
SRC_PATH := src
OBJ_PATH := build
BIN_PATH := bin
TESTS_PATH := src/tests
INCLUDE_PATH := src/include

# file macros
SOURCES := $(wildcard $(SRC_PATH)/*.c)
OBJECTS := $(patsubst $(SRC_PATH)/%.c, $(OBJ_PATH)/%.o, $(SOURCES))
EXECUTABLE := $(BIN_PATH)/haystack

default: $(PATHS) $(EXECUTABLE)

# link object files and produce executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(FLAGS) $^ -o $@

# build all object files including main.o
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) $(FLAGS) -I$(SRC_PATH) -c $< -o $@

# create directories if non-existent
$(PATHS):
	@mkdir $@

# PHONY targets
.PHONY: clean
clean:
	rm $(OBJECTS) $(BIN_PATH)/$(EXECUTABLE)