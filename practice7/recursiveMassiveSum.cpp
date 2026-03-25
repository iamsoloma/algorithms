//Реализовать рекурсивно сумму чисел массива

#include <stdio.h>

int sum(int nums[], int res, int lenght) {
    if (lenght == 0) {
        return nums[0];
    }
    return res + nums[lenght] + sum(nums, res, lenght - 1);
}

int main() {
    int mas[] = {1, 2, 3, 4, 5};
    int lenght = 5;

    for (int i = 0; i<lenght; i++) {
        if (i == lenght-1) {
            printf("%d", mas[i]);
        } else {
            printf("%d + ", mas[i]);
        }
        
    }

    int res = sum(mas, 0, lenght);

    printf(" = %d\n", res);
}