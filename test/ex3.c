/**
 * @file ex3.c
 * @brief pointer test
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */

int main(){
    int a = 2;
    int b = 3;
    printf("%d, %d\n", a, b);
    int* pa = &a;
    int* pb = &b;
    int t = *pb;
    *pb = *pa;
    *pa = t;
    printf("%d, %d\n", a, b);
    return 0;
}