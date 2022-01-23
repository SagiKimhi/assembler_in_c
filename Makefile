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
BIN=asmake

all: withTable withList $(BIN)

withList: CFLAGS+=-D__USE_LIST__
withList: $(BIN)

withTable: CFLAGS+=-D__USE_TABLE__
withTable: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c $(HDRS)
	$(CC) $(CFLAGS) $(OFLAGS) $< -o $@ $(LIB)

$(OBJ):
	mkdir $@

clean:
	$(RM) $(OBJ)/*.o $(BIN)
