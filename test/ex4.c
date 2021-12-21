/**
 * @file ex1.c
 * @brief common test: symbol_table
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */

int main() {
    int i = 3, j = 1;
    {
        int i = 0;
        int a, b;
        a = j;
        {
            int c = i;
            i = 2;
        }
    }
    return 0;
}