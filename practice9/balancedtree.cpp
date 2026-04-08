#include <stdio.h>
#include <stdlib.h>

// Структура узла AVL-дерева
typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

/* ------------------- Вспомогательные функции ------------------- */

// Возвращает максимум из двух чисел
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Возвращает высоту узла (0 для NULL)
int height(Node* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Вычисляет коэффициент баланса узла (разница высот левого и правого поддеревьев)
int getBalance(Node* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Находит узел с минимальным значением (самый левый)
Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

/* ------------------- Функции поворотов ------------------- */

// Правый поворот (для левого дисбаланса)
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Выполнить поворот
    x->right = y;
    y->left = T2;

    // Обновить высоты
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Вернуть новый корень
    return x;
}

// Левый поворот (для правого дисбаланса)
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Выполнить поворот
    y->left = x;
    x->right = T2;

    // Обновить высоты
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Вернуть новый корень
    return y;
}

/* ------------------- Основные операции ------------------- */

// Создание нового узла
Node* createNode(int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // Новый узел изначально лист
    return node;
}

// Вставка значения в дерево с балансировкой
Node* insert(Node* node, int value) {
    // 1. Обычная вставка как в BST
    if (node == NULL)
        return createNode(value);

    if (value < node->value)
        node->left = insert(node->left, value);
    else if (value > node->value)
        node->right = insert(node->right, value);
    else // Дубликаты не допускаются
        return node;

    // 2. Обновить высоту предка
    node->height = 1 + max(height(node->left), height(node->right));

    // 3. Получить коэффициент баланса для проверки баланса этого узла
    int balance = getBalance(node);

    // Если узел несбалансирован, то есть 4 случая:

    // Левый Левый (Left Left Case)
    if (balance > 1 && value < node->left->value)
        return rightRotate(node);

    // Правый Правый (Right Right Case)
    if (balance < -1 && value > node->right->value)
        return leftRotate(node);

    // Левый Правый (Left Right Case)
    if (balance > 1 && value > node->left->value) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Правый Левый (Right Left Case)
    if (balance < -1 && value < node->right->value) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Вернуть неизмененный указатель на узел
    return node;
}

// Удаление значения из дерева с балансировкой
Node* deleteNode(Node* root, int value) {
    if (root == NULL)
        return root;

    // 1. Стандартное удаление из BST
    if (value < root->value)
        root->left = deleteNode(root->left, value);
    else if (value > root->value)
        root->right = deleteNode(root->right, value);
    else {
        // Узел с одним ребенком или без детей
        if ((root->left == NULL) || (root->right == NULL)) {
            Node* temp = root->left ? root->left : root->right;
            // Нет детей
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else // Один ребенок
                *root = *temp; // Копируем содержимое непустого ребенка
            free(temp);
        } else {
            // Узел с двумя детьми: получаем преемника (наименьший в правом поддереве)
            Node* temp = minValueNode(root->right);
            root->value = temp->value;
            root->right = deleteNode(root->right, temp->value);
        }
    }

    if (root == NULL)
        return root;

    // 2. Обновить высоту текущего узла
    root->height = 1 + max(height(root->left), height(root->right));

    // 3. Получить коэффициент баланса для проверки баланса этого узла
    int balance = getBalance(root);

    // Если узел несбалансирован, то есть 4 случая:

    // Левый Левый (Left Left Case)
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Левый Правый (Left Right Case)
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Правый Правый (Right Right Case)
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Правый Левый (Right Left Case)
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Поиск значения в дереве (возвращает 1 если найдено, 0 иначе)
int search(Node* root, int value) {
    if (root == NULL)
        return 0;
    if (root->value == value)
        return 1;
    if (value < root->value)
        return search(root->left, value);
    else
        return search(root->right, value);
}

/* ------------------- Функции обхода ------------------- */
void preOrder(Node *root) {
   if(root != NULL) {
       printf("%d ", root->value);
       preOrder(root->left);
       preOrder(root->right);
   }
}
void inOrder(Node *root) {
   if(root != NULL) {
       inOrder(root->left);
       printf("%d ", root->value);
       inOrder(root->right);
   }
}
void postOrder(Node *root) {
   if(root != NULL) {
       postOrder(root->left);
       postOrder(root->right);
       printf("%d ", root->value);
   }
}
void print2DUtil(Node *root, int space) {
   if (root == NULL)
       return;
   space += 5;
   print2DUtil(root->right, space);
   printf("\n");
   for (int i = 5; i < space; i++)
       printf(" ");
   printf("%d\n", root->value);
   print2DUtil(root->left, space);
}
void print2D(Node *root) {
   print2DUtil(root, 0);
}
/* -------------------------------------------------------- */


int main() {
    Node* root = NULL;

     /* Построение дерева */
     root = insert(root, 10);
     root = insert(root, 20);
     root = insert(root, 30);
     root = insert(root, 40);
     root = insert(root, 50);
     root = insert(root, 25);
 
     /* Сбалансированное дерево будет следующим:
             30
            /  \
          20   40
         /  \     \
        10  25     50
     */
 
     printf("Прямой обход дерева: ");
     preOrder(root);
     printf("\n\nВизуализация дерева:\n");
     print2D(root);
     
     printf("\n\nУдаление значения 10\n");
     root = deleteNode(root, 10);
     
     printf("Прямой обход после удаления: ");
     preOrder(root);
     
     printf("\n\nПоиск значения 40: %s\n", search(root, 40) ? "Найдено" : "Не найдено");
     printf("Поиск значения 99: %s\n", search(root, 99) ? "Найдено" : "Не найдено");
 
     return 0;
}