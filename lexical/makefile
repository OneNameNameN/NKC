nkc: nkc.l nkc.y
	bison -d nkc.y
	flex nkc.l
	g++ -w -o nkc nkc.tab.c lex.yy.c ../grammer/nkc.c ../grammer/symbolTable.cpp

clean:
	rm -rf lex.yy.c nkc.tab.h nkc.tab.c nkc.exe nkc
