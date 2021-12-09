nkc: nkc.l nkc.y
	bison -d nkc.y
	flex nkc.l
	g++ -w -o nkc nkc.tab.c lex.yy.c ../grammer/Node/*.cpp ../grammer/*.cpp ../grammer/Intermediate/*.cpp

clean:
	rm -rf lex.yy.c nkc.tab.h nkc.tab.c nkc.exe nkc target target.asm target.obj program.exe program
