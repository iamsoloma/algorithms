//Реализовать рекурсивно вычисление факториала

#include <stdio.h>

int factorial(int degree) {
    if (degree == 1) {
        return 1;
    } else {
        return degree * factorial(degree-1);
    }
}

int main() {
    int res = factorial(5);
    printf("5! = %d\n", res);
    res = factorial(25);
    printf("25! = %d\n", res);
}