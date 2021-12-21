/**
 * @file ex1.c
 * @brief common test: ast
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */

int main() {
    int i = 1;
    while (i < 10) {
        if(i < 5)
            i = i + 2;
        i = i * 2;
    }
    printf("%d", i);
    return 0;
}