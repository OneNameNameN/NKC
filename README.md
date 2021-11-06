# NKC
南开大学软件学院

- Flex

  在命令行输入

  > flex lex.l
  >
  > gcc -o output.exe lex.yy.c
  >
  > output.exe  +  " data.txt 文件所在地址"

  即可得到语法分析结果
  
  
  
  最终解释权归 shygo 所有

## 语法分析器简单示例

***运行方法***

1. ```flex lex.l```
2. ```bison -dy yacc.y```
3. ```gcc y.tab.c lex.yy.c -o a.exe```
4. ```a.exe ./test/test.txt``` 此文件路径可能由于终端以及系统不同略有差异，如windows路径为.\test\test.txt  

***注意事项***

1. 运行第三步会有众多warning，不影响生成程序，不要大惊小怪
2. 此程序仅为示例，有众多规则仍需编写，希望hzy和zwl加油w