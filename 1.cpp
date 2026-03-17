//Найдите средний узел односвязного списка. Если количество узлов четное, вернуть второй из двух средних.

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

struct Node
{
    int data;
    struct Node* next;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));

    if (newNode == NULL) {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }

    newNode->data=data;
    newNode->next = NULL;
    return newNode;
};

struct Node* insertFirst(struct Node* head, int data) {
    struct Node* newNode = createNode(data);
    if (newNode == NULL) return head;

    newNode->next = head;
    return newNode;

};

struct Node* insertLast(struct Node* head, int data) {
    struct Node* newNode = createNode(data);
    struct Node* current = head;

    while (current->next != NULL) {
        current=current->next;
    }

    current->next = newNode;
    return head;
};

struct Node* search(struct Node* head, int key) {
    struct Node* current = head;

    while (current->next != NULL) {
        if (current->data == key) {
            return current;
        };
        current=current->next;
    }

    return NULL;
}

struct Node* deleteNode(struct Node* head, int key) {
    if (head == NULL) return NULL;
    
    // Если удаляем первый элемент
    if (head->data == key) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
        return head;
    }
    
    // Поиск элемента для удаления
    struct Node* current = head;
    while (current->next != NULL) {
        if (current->next->data == key) {
            struct Node* temp = current->next;
            current->next = current->next->next;
            free(temp);
            return head;
        }
        current = current->next;
    }
    
    printf("Элемент %d не найден\n", key);
    return head;
}

void printList(struct Node* head) {
    if (head == NULL) {
        printf("Список пуст\n");
        return;
    }
    
    printf("Список: ");
    struct Node* current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}


int getLength(struct Node* head) {
   if (head == NULL) {
    return 0;
   };

   int count = 1;

   struct Node* current = head;
   while (current->next != NULL) {
    current=current->next;
    count+=1;
   };

   return count;
};

// Освобождение памяти
void freeList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
}

struct Node* getMiddle(struct Node* head) {
    int length = getLength(head);
    struct Node* current = head;
    int middleAddr = 0;

    if (length % 2 == 0) {
        middleAddr = length / 2;
    } else {
        middleAddr = length / 2;
    }
    for (int i = 0; i < middleAddr; i++) {
        if (current->next == NULL){
            return NULL;
        }
        current = current->next;
    }

    return current;
}

int main() {
    struct Node* head = NULL;

    for (int i = 0; i < 10; i++) {
        head = insertFirst(head, i);
    };

    for (int i = 1; i < 10; i++) {
        head = insertLast(head, i);
    };

    printList(head);

    struct Node* middle = getMiddle(head);
    if (middle != NULL) {
        int data = middle->data;
        std::cout << "Middle node: " << data << std::endl;
    }

    return 0;
}