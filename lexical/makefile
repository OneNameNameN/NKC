nkc: nkc.l nkc.y
	bison -d nkc.y
	flex nkc.l
	gcc -o nkc nkc.tab.c lex.yy.c ../grammer/nkc.c
