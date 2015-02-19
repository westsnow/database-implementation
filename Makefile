CC = g++ -O2 -Wno-deprecated 

GoogleTestDir = ./GTestLib

tag = -i
llflag = -ll

ifdef linux
tag = -n
llflag = -lfl
endif


gtestlib = ./lib

BIN = ./bin/
SRC = ./source/
GSRC = ./gtest/


GLIST = GtestLoad.o
GOBJ_FILES = $(addprefix $(BIN), $(GLIST))

LIST = Record.o Comparison.o ComparisonEngine.o Schema.o File.o DBFile.o y.tab.o lex.yy.o Pipe.o BigQ.o
OBJ_FILES = $(addprefix $(BIN), $(LIST))


all: test

gtest: $(OBJ_FILES) $(GOBJ_FILES) $(BIN)gtest.o 
	$(CC) -o $(BIN)$@ $(OBJ_FILES) $(GOBJ_FILES) $(BIN)gtest.o -I$(GoogleTestDir)/include -L$(GoogleTestDir)/lib -lgtest -lpthread

test: $(OBJ_FILES) $(BIN)test.o
	$(CC) -o $(BIN)$@ $(OBJ_FILES) ./bin/test.o $(llflag)

main: $(OBJ_FILES) $(BIN)main.o
	$(CC) -o $(BIN)$@ $(OBJ_FILES) ./bin/main.o $(llflag)


bin/%.o: $(SRC)%.cc
	$(CC) -c -g $< -o $@

bin/%.o: $(GSRC)%.cc
	$(CC) -c -g $< -o $@ -I$(GoogleTestDir)/include -L$(GoogleTestDir)/lib -lgtest -lpthread

bin/y.tab.o: $(SRC)Parser.y
	yacc -d $(SRC)Parser.y -o $(SRC)y.tab.c
	g++ -c $(SRC)y.tab.c -o $@

bin/lex.yy.o: $(SRC)Lexer.l
	lex  $(SRC)Lexer.l
	mv lex.yy.c $(SRC)lex.yy.c
	gcc  -c $(SRC)lex.yy.c -o $@
clean: 
	rm -f $(BIN)*
	rm -f $(SRC)y.tab.c
	rm -f $(SRC)lex.yy.c
	rm -f $(SRC)y.tab.h
	rm -f ./data/testTable
	rm -f ./data/itemTable
	rm -f ./data/caritem
	rm -f ./data/*.bin
