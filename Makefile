CC = gcc
CFLAGS = -Wall -O3 -g --std=c99
INCLUDE = -I.

BIN = ./bin
SRC = ./src
OBJ = ./obj

OBJ_TEST = $(OBJ)/test.o
OBJS = $(OBJ)/word.o $(OBJ)/gate.o $(OBJ)/adder.o $(OBJ)/alu.o $(OBJ)/reg_file.o $(OBJ)/memory.o  $(OBJ)/control_unit.o $(OBJ)/mips.o $(OBJ_TEST)
# $(OBJ)/adder.o $(OBJ)/alu.o $(OBJ)/reg_file.o $(OBJ)/memory.o $(OBJ)/control_unit.o $(OBJ)/mips.o

.PHONY: all clean

all:
	make test

test: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $(BIN)/$@ $(OBJS)

$(OBJ)/%.o : $(SRC)/%.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $@ $<

clean:
	rm -rf $(BIN)/*
	rm -rf $(OBJ)/*
