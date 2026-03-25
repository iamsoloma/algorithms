//Реализовать Ханойские башни

#include <stdio.h>
#include <stdbool.h>  // для bool

#define MAX_SIZE 5  // максимальный размер стека

// Структура стека 
struct Stack {
    int data[MAX_SIZE];  // массив для хранения элементов
    int top;             // индекс верхнего элемента (-1 если пуст)
};

// Инициализация стека
void initStack(struct Stack* s) {
    s->top = -1;  // стек пуст
}

// Проверка на пустоту
bool isEmpty(struct Stack* s) {
    return s->top == -1;
}

// Проверка на заполненность
bool isFull(struct Stack* s) {
   return s->top == MAX_SIZE - 1;
}

// Добавление элемента на вершину
void push(struct Stack* s, int value) {
    if (isFull(s)) {
        printf("Error: stack overlow! Impossible to add %d\n", value);
        return;
    }
        
    // Явно увеличиваем индекс на 1
    s->top = s->top + 1;
    
    // Теперь кладем значение на новую позицию
    s->data[s->top] = value;
    printf("Added element: %d\n", value);
}

// Удаление элемента с вершины
int pop(struct Stack* s) {
   if (isEmpty(s)) {
    printf("Error: stack is empty! Impossible to delete.");
   }

   int value = s->data[s->top];
   s->top = s->top-1;

   return value;
}

// Просмотр верхнего элемента без удаления
int peek(struct Stack* s) {
    if (isEmpty(s)) {
        printf("Error: stack is empty! Impossible to peek\n");
        return -1;
    }
    return s->data[s->top];
}

// Получение размера стека
int size(struct Stack* s) {
    return s->top + 1;
}

// Вывод всех элементов стека
void display(struct Stack* s) {
    if (isEmpty(s)) {
        printf("Стек пуст\n");
        return;
    }
    
    printf("Stack: ");
    for (int i = s->top; i >= 0; i--) {
        printf("%d ", s->data[i]);
    }
    printf("\n");
}

int main() {
    // Создаем 3 башни
    struct Stack leftTower;
    initStack(&leftTower);
    struct Stack middleTower;
    initStack(&middleTower);
    struct Stack rightTower;
    initStack(&rightTower);
    
    // Заполняем 1 башню
    for (int i = 1; i < 6; i++) {
        push(&leftTower, i);
    }

    return 0;
}
