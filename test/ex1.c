/**
 * @file ex1.c
 * @brief error test
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */

int main() {
    int m = 2;
    // handle IO
    scanf("%d", &m);
    printf("%d\n", (m+100) % 2);

    int i = 0;
    int factorial = 3;
    int result = 1;
    whlie (factorial > 0) {  // factorial
        result = result * factorial;
        factorial = factorial - 1;
    }
    printf("%d\n", result % 100);

    for (int j = 0; j < result % 100 - 1; j++) {
        for (int k = j + 1; k < result % 100 - 1; k++) {
            /*
            printf("Hello ~: %d", j);
            */
            if (k - 5 == j || ! j + 2 == k && k - 3 == j) {
                i = k % (j + 1);
                printf("1: %d\n", i);
            } else if (k == j + 3 && k % 2 == 0) {
                i = k / (j + 1);
                printf("2: %d\n", i);
            } else {
                printf("3: %d\n", i);
                /*
                printf("Aha~ %d", j);
                */
            }
        }
    }
    return 0;
}