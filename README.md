# NKC
南开大学软件学院

文件格式：

/lexical 包含词法分析器和语法分析器

/grammer 包含抽象语法树

/test    包含测试样例

## 生成语法分析器可执行文件

1. 进入/lexical目录
2. make(linux) mingw32-make(windows)
3. nkc>result.txt可在result.txt看到分析结果
4. /lexical目录下target.asm为目标汇编代码，program.exe为目标程序

## 注意事项

1. make所需系统配置：flex，bison，gcc
1. make命令为linux命令，windows用户请配置使用mingw32-make
2. windows用户若make时错误，请尝试将bison的m4.exe添加至/lexical目录
3. nkc运行时需要系统配置：nasm, gcc（32位）
3. 若nkc运行时报错，检查nasm与gcc（32位）是否配置成功。例如mingw64/bin路径覆盖mingw32/bin路径时无法找到mingw32/bin中的gcc，配置失败
