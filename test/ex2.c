/**
 * @file ex2.c
 * @brief array test
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */

int main() {
    int a[5];
    int i=0;
    int b;
    for (i; i < 5; i=i+1) {
        a[i] = i;
    }
    i = 0;
    for (i; i < 5; i=i+1) {
        printf("%d", a[i]);
    }
    return 0;
}