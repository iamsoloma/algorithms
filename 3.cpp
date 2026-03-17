//Напишите рекурсивную функцию для возведения числа a в степень n (a^n).

#include <stdio.h>


int pow(int base, int degree, int res, int curr_degre) {
    if (curr_degre == degree) {
        return res;
    }
    res *= base;
    return pow(base, degree, res, curr_degre+1);
}

/*int pow(int base, int degree) {
    int res = 0;
    for (int i = 1; i<degree; i++) {
        if (i == 1) {
            res = base * base;
        } else {
            res *= base;
        }
    }

    return res;
}*/

int main() {
    int res = pow(2, 10, 2, 1);
    printf("%d ^ %d = %d\n", 2, 10, res);
}