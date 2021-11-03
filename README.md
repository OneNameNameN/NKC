# NKC
南开大学软件学院

- Flex

  修改第 78 行的 fopen 文件为下载到本地的 data.txt 文件

  ```
  yyin=fopen("O:/data.txt","r");
  ```

  在命令行输入

  > flex lex.l
  >
  > gcc -o output.exe lex.yy.c
  >
  > output.exe

  即可得到以下结果

  > line1:(type, void)
  > line1:(keyword, main)
  > line1:(left parenthesis, ()
  > line1:(right parenthesis, ))
  > line1:(left brace, {)
  > line2:(type, int)
  > line2:(identity, a)
  > line2:(assign, =)
  > line2:(integer, 0)
  > line2:(semicolon, ;)
  > line3:(type, int)
  > line3:(identity, i)
  > line3:(semicolon, ;)
  > line4:(keyword, if)
  > line4:(left parenthesis, ()
  > line4:(left not, !)
  > line4:(identity, a)
  > line4:(right parenthesis, ))
  > line4:(left brace, {)
  > line5:(identity, i)
  > line5:(assign, =)
  > line5:(integer, 1)
  > line5:(semicolon, ;)
  > line6:(right brace, })
  > line7:(right brace, })
