CC=gcc
CFLAGS=-ansi -pedantic -g -Wall
OFLAGS=-c

HDR=hdr
SRC=src
OBJ=obj
LIB=-I$(HDR)
HDRS=$(wildcard $(HDR)/*.h)
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
BIN=main

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ)/%.o: $(SRC)/%.c $(HDR)/%.h
	$(CC) $(CFLAGS) $(OFLAGS) $< -o $@ $(LIB)

$(OBJ):
	mkdir $@

clean:
	$(RM) -r $(OBJ)/* $(BIN)