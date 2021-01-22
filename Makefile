PROJECT=renderer

INCLUDE=include
SRC=src
OBJ=obj
BIN=bin

CC=cc
AR=ar
CFLAGS=-std=c11 -O2 -I$(INCLUDE) -Wall -g -fopenmp
LFLAGS= -lm -fopenmp

SRC_FILES=$(wildcard $(SRC)/*.c)
OBJ_FILES=$(addprefix $(OBJ)/,$(notdir $(SRC_FILES:.c=.o)))

CGLM_SRC=$(wildcard third_party/cglm/*.c)
CGLM_OBJ=$(notdir $(CGLM_SRC:.c=.o))

$(BIN)/$(PROJECT): $(OBJ_FILES) $(OBJ)/cglm.a
	@mkdir -p $(BIN)
	$(CC) $(LFLAGS) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(OBJ)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ)/cglm.a: $(CGLM_SRC)
	$(CC) $(CFLAGS) $^ -c
	$(AR) rcs $@ $(CGLM_OBJ)
	rm -f $(CGLM_OBJ)

.PHONY: clean cleanall

clean:
	@rm -rf $(BIN)/$(PROJECT) $(OBJ_FILES)

cleanall:
	@rm -rf $(BIN)/$(PROJECT) $(OBJ_FILES) $(OBJ)/cglm.a
