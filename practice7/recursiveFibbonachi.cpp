//Реализовать рекурсивно вычисление n-ного числа фибоначчи

#include <stdio.h>

int fibbonachi(int n) {
    if (n == 0) {
        return 0;
    } else if (n == 1) {
        return 1;
    } else {
        return fibbonachi(n -1) + fibbonachi(n-2);
    }
}

int main() {
    int res = fibbonachi(10);
    printf("fibbonachi(10) = %d\n", res);
}