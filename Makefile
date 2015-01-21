CC = g++ -O2 -Wno-deprecated 
SRC = src/
OBJ = obj/
tag = -i

ifdef linux
tag = -n
endif

test.out: Record.o Comparison.o ComparisonEngine.o Schema.o File.o DBFile.o y.tab.o lex.yy.o test.o
	$(CC) -o test.out Record.o Comparison.o ComparisonEngine.o Schema.o File.o DBFile.o y.tab.o lex.yy.o test.o
	
main: Record.o Comparison.o ComparisonEngine.o Schema.o File.o y.tab.o lex.yy.o main.o
	$(CC) -o main $(OBJ)Record.o $(OBJ)Comparison.o $(OBJ)ComparisonEngine.o $(OBJ)Schema.o $(OBJ)File.o $(OBJ)y.tab.o $(OBJ)lex.yy.o $(OBJ)main.o 
	
test.o: $(SRC)test.cc
	$(CC) -g -c $(SRC)test.cc -o $(OBJ)test.o

main.o: $(SRC)main.cc
	$(CC) -g -c $(SRC)main.cc -o $(OBJ)main.o
	
Comparison.o: $(SRC)Comparison.cc
	$(CC) -g -c $(SRC)Comparison.cc -o $(OBJ)Comparison.o
	
ComparisonEngine.o: $(SRC)ComparisonEngine.cc
	$(CC) -g -c $(SRC)ComparisonEngine.cc -o $(OBJ)ComparisonEngine.o
	
DBFile.o: $(SRC)DBFile.cc
	$(CC) -g -c $(SRC)DBFile.cc -o $(OBJ)DBFile.o

File.o: $(SRC)File.cc
	$(CC) -g -c $(SRC)File.cc -o $(OBJ)File.o

Record.o: $(SRC)Record.cc
	$(CC) -g -c $(SRC)Record.cc -o $(OBJ)Record.o

Schema.o: $(SRC)Schema.cc
	$(CC) -g -c $(SRC)Schema.cc -o $(OBJ)Schema.o
	
y.tab.o: $(SRC)Parser.y
	yacc -d $(SRC)Parser.y 
	#sed -n y.tab.c -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/" 
	g++ -c y.tab.c -o $(OBJ)y.tab.o

lex.yy.o: $(SRC)Lexer.l
	lex  $(SRC)Lexer.l
	gcc  -c lex.yy.c -o $(OBJ)lex.yy.o

clean: 
	rm -f $(OBJ)*.o
	rm -f $(OBJ)*.out
	rm -f y.tab.c
	rm -f lex.yy.c
	rm -f y.tab.h
