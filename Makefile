PROJECT=renderer

INCLUDE=include
SRC=src
OBJ=obj
BIN=bin

CC=cc
CFLAGS=-std=c99 -O2 -I$(INCLUDE)
LFLAGS=

SRC_FILES=$(wildcard $(SRC)/*.c)
OBJ_FILES=$(addprefix $(OBJ)/,$(notdir $(SRC_FILES:.c=.o)))

$(BIN)/$(PROJECT): $(OBJ_FILES)
	@mkdir -p $(BIN)
	$(CC) $(LFLAGS) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(OBJ)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	@rm -rf $(BIN)/$(PROJECT) $(OBJ_FILES)
