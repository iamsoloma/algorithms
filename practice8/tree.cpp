#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Структура узла бинарного дерева
typedef struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
} Node;

// Создание нового узла
Node *createNode(int value)
{
    Node *newNode = (Node *)malloc(sizeof(Node));

    if (newNode == NULL)
    {
        printf("Ошибка выделения памяти.");
        return NULL;
    }
    newNode->data = value;

    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Вставка элемента в бинарное дерево поиска (рекурсивная)
Node *insert(Node *root, int value)
{
    if (root == NULL)
    {
        return createNode(value);
    }

    if (value < root->data)
    {
        root->left = insert(root->left, value);
    }
    if (value > root->data)
    {
        root->right = insert(root->right, value);
    }

    return root;
}

// Поиск элемента в дереве (рекурсивный)
bool search(Node *root, int value)
{

    if (root == NULL)
    {
        return false;
    }

    if (root->data == value)
    {
        return true;
    }

    if (value < root->data)
    {
        return search(root->left, value);
    }
    else if (value > root->data)
    {
        return search(root->right, value);
    }

    return false;
}

// Поиск минимального значения в поддереве
Node *findMin(Node *root)
{
    Node *current = root;
    while (current->left != NULL)
    {
        current = current->left;
    }
    return current;
}

// Поиск максимального значения в поддереве
Node *findMax(Node *root)
{
    Node *current = root;
    while (current->right != NULL)
    {
        current = current->right;
    }
    return current;
}

// Удаление элемента из дерева
Node *deleteNode(Node *root, int value)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (value < root->data)
    {
        root->left = deleteNode(root->left, value);
    }
    else if (value > root->data)
    {
        root->right = deleteNode(root->right, value);
    }
    else
    {
        if (root->left == NULL && root->right == NULL)
        {
            free(root);
            return NULL;
        }
        else if (root->left == NULL)
        {
            Node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            Node *temp = root->left;
            free(root);
            return temp;
        }
        else
        {
            Node *minRight = findMin(root->right);
            root->data = minRight->data;
            root->right = deleteNode(root->right, minRight->data);
        }
    }
    return root;
}

// Очистка всего дерева
void clearTree(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    clearTree(root->left);
    clearTree(root->right);
    free(root);
}

// Проверка на пустоту
bool isEmpty(Node *root)
{
    return root == NULL;
}

void preOrder(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    printf("%d ", root->data);
    preOrder(root->left);
    preOrder(root->right);
}

void inOrder(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    inOrder(root->left);
    printf("%d ", root->data);
    inOrder(root->right);
}

void postOrder(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    postOrder(root->left);
    postOrder(root->right);
    printf("%d ", root->data);
}

// Структура для очереди (для BFS)
#define MAX_SIZE 1000
typedef struct {
    Node* items[MAX_SIZE];
    int front, rear;
} Queue;

// Инициализация очереди
void initQueue(Queue* q) {
    q->front = q->rear = 0;
}

// Проверка на пустоту
int isEmpty(Queue* q) {
    return q->front == q->rear;
}

// Добавление в очередь
void enqueue(Queue* q, Node* node) {
    q->items[q->rear++] = node;
}

// Извлечение из очереди
Node* dequeue(Queue* q) {
    return q->items[q->front++];
}

void bfs(Node* root)
{
    if (root == NULL) {return;}
    Queue q;
    initQueue(&q);

    enqueue(&q, root);

    while (!isEmpty(&q))
    {
        Node *node = dequeue(&q);
        if (node->left != NULL)
        {
            enqueue(&q, node->left);
        }
        if (node->right != NULL)
        {
            enqueue(&q, node->right);
        }

        printf("%d ", node->data);
    }
}

int main()
{
    Node *root = NULL;

    // Вставка элементов
    int values[] = {50, 30, 70, 20, 40, 60, 80, 15, 25, 35, 45, 55, 65, 75, 85};
    int n = sizeof(values) / sizeof(values[0]);

    printf("Вставка элементов: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", values[i]);
        root = insert(root, values[i]);
    }
    printf("\n\n");

    // Поиск элементов
    printf("Поиск элементов:\n");
    int searchValues[] = {40, 90, 25, 100};
    for (int i = 0; i < 4; i++)
    {
        if (search(root, searchValues[i]))
        {
            printf("   %d найден в дереве\n", searchValues[i]);
        }
        else
        {
            printf("   %d не найден в дереве\n", searchValues[i]);
        }
    }
    printf("\n");

    // Нахождение минимального и максимального элементов
    Node *min = findMin(root);
    Node *max = findMax(root);
    printf("Минимальный элемент: %d\n", min->data);
    printf("Максимальный элемент: %d\n", max->data);
    printf("\n");

    // Удаление элементов
    printf("Удаление элементов:\n");
    int deleteValues[] = {20, 50, 70};
    for (int i = 0; i < 3; i++)
    {
        printf("   Удаление %d: ", deleteValues[i]);
        root = deleteNode(root, deleteValues[i]);
        printf("In-order обход после удаления: ");
        inOrder(root);
        printf("\n");
    }
    printf("\n");

    // Очистка дерева
    printf("Очистка дерева...\n");
    clearTree(root);
    root = NULL;

    if (isEmpty(root))
    {
        printf("Дерево успешно очищено.\n");
    }

    // Пример создания дерева:
    //       1
    //     /   \
    //    2     3
    //   / \   /
    //  4   5 6

    printf("Создание vroot:\n");
    Node *vroot = createNode(1);
    vroot->left = createNode(2);
    vroot->right = createNode(3);
    vroot->left->left = createNode(4);
    vroot->left->right = createNode(5);
    vroot->right->left = createNode(6);

    printf("Прямой обход (pre-order): ");
    preOrder(vroot);
    printf("\n");

    printf("Симметричный обход (in-order): ");
    inOrder(vroot);
    printf("\n");

    printf("Обратный обход (post-order): ");
    postOrder(vroot);
    printf("\n");

    printf("Обход в ширину (BFS): ");
    bfs(vroot);
    printf("\n");

    return 0;
}