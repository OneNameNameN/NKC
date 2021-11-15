nkc: nkc.l nkc.y
	bison -d nkc.y
	flex nkc.l
	gcc -o nkc nkc.tab.c lex.yy.c ../grammer/nkc.c

clean:
	rm -rf lex.yy.c nkc.tab.h nkc.tab.c nkc.exe nkc