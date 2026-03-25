//Извлеките первые K элементов из очереди и верните их в обратном порядке.

#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 100

// Структура очереди на массиве
struct Queue {
    int data[MAX_SIZE];  // массив для хранения элементов
    int head;            // индекс начала очереди (откуда удаляем)
    int tail;            // индекс конца очереди (куда добавляем)
    int count;           // текущее количество элементов
};

// Инициализация очереди
void initQueue(struct Queue* q) {
    q->head = 0;
    q->tail = -1;  // очередь пуста
    q->count = 0;
}

// Проверка на пустоту
bool isEmpty(struct Queue* q) {
    return q->count == 0;
}

// Проверка на заполненность
bool isFull(struct Queue* q) {
    return q->count == MAX_SIZE;
}

// Добавление элемента в конец очереди (enqueue)
void enqueue(struct Queue* q, int value) {
    if (isFull(q)) {
        printf("Ошибка: очередь переполнена! Невозможно добавить %d\n", value);
        return;
    }
    
    // Вычисляем новую позицию для tail с учетом кольцевости
    // tail = (tail + 1) % MAX_SIZE
    int newTail = q->tail + 1;
    if (newTail >= MAX_SIZE) {
        newTail = 0;  // заворачиваем в начало
    }
    
    // Обновляем tail и добавляем элемент
    q->tail = newTail;
    q->data[q->tail] = value;
    
    // Увеличиваем счетчик
    q->count = q->count + 1;
    
    printf("Добавлен элемент: %d (tail = %d)\n", value, q->tail);
}

// Удаление элемента из начала очереди (dequeue)
int dequeue(struct Queue* q) {
    if (isEmpty(q)) {
        printf("Ошибка: очередь пуста! Невозможно удалить\n");
        return 0;
    }

    int tmp = q->data[q->head];

    q->head = (q->head + 1) % MAX_SIZE;
    q->count -= 1;

    return tmp;
}

// Просмотр первого элемента (без удаления)
int front(struct Queue* q) {
    if (isEmpty(q)) {
        printf("Ошибка: очередь пуста!\n");
        return -1;
    }
    return q->data[(q->head)%MAX_SIZE];
}

// Просмотр последнего элемента
int back(struct Queue* q) {
   if (isEmpty(q)) {
    printf("Ошибка: очередь пуста!\n");
    return -1;
   }

   return q->data[(q->tail)%MAX_SIZE];
}

// Получение размера очереди
int size(struct Queue* q) {
   return q->count;
}

// Вывод всех элементов очереди
void display(struct Queue* q) {
    if (isEmpty(q)) {
        printf("Очередь пуста\n");
        return;
    }
    
    printf("Очередь (от начала к концу): ");
    printf("[head=%d, tail=%d, count=%d] ", q->head, q->tail, q->count);
    
    // Проходим по всем элементам от head до tail с учетом кольцевости
    int index = q->head;
    for (int i = 0; i < q->count; i = i + 1) {
        printf("%d ", q->data[index]);
        
        // Переходим к следующему индексу с учетом кольцевости
        index = index + 1;
        if (index >= MAX_SIZE) {
            index = 0;
        }
    }
    printf("\n");
}

void reverseKElems(struct Queue* q, int k) {
    if (k < 0 || k > q->count) {
        printf("Запрос элементов некоретен.");
        return;
    }
    int tmp[k];

    for (int i = 0; i<k; i++) {
        int first = front(q);
        tmp[i] = first;
        dequeue(q);
    }

    for (int i = k-1; i>=0; i--) {
        enqueue(q, tmp[i]);
    }
    
}

int main() {
    struct Queue queue;
    initQueue(&queue);
    
    for (int i = 0; i < 10; i++) {
        enqueue(&queue, i);
    }

    display(&queue);

    reverseKElems(&queue, 5);

    display(&queue);
    
    

    return 0;
}