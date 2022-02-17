# ---------------------------------------------
#  Variables:
#  --------------------------------------------
# Compiler
CC=gcc
# Flags
CFLAGS=-ansi -pedantic -g -Wall
OFLAGS=-c
LIB=-I$(HDR)
# Directories
HDR=hdr
SRC=src
OBJ=obj
# Files
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
BIN=asmake

# ---------------------------------------------
#  Compilation:
#  --------------------------------------------
# Default binary compilation method
all: withTree

# The program's data structure of
# choice would be a Linked List structure
withList: CFLAGS+=-D__USE_LIST__
withList: $(BIN)

# The program's data structure of
# choice would be a Hash Table structure
withTable: CFLAGS+=-D__USE_TABLE__
withTable: $(BIN)

# The program's data structure of
# choice would be a Binary Tree structure
withTree: CFLAGS+=-D__USE_TREE__
withTree: $(BIN)

# Creation of binary file
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

# Creation of object files
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $(OFLAGS) $< -o $@ $(LIB)

# Creates an object directory to store object files
# no operation is done if one already exists
$(OBJS):
	mkdir -p $(OBJ)

# Removes object files and executable/binary file
clean:
ifdef OBJ
	$(RM) $(OBJ)/*.o
endif
ifdef BIN
	$(RM) $(BIN)
endif
