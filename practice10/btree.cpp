#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define T 3 // Минимальная степень дерева (каждый узел содержит от T-1 до 2T-1 ключей)
#define MAX_KEYS (2 * T - 1)
#define MIN_KEYS (T - 1)

// Структура узла B-дерева
typedef struct BTreeNode
{
    int keys[MAX_KEYS];                       // Массив ключей
    struct BTreeNode *children[MAX_KEYS + 1]; // Дочерние узлы
    int num_keys;                             // Текущее количество ключей
    bool leaf;                                // Является ли узел листом
} BTreeNode;

// Создание нового узла
BTreeNode *create_node(bool leaf)
{
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    if (!node)
    {
        printf("Ошибка выделения памяти\n");
        return NULL;
    }

    node->num_keys = 0;
    node->leaf = leaf;

    for (int i = 0; i <= MAX_KEYS; i++)
    {
        node->children[i] = NULL;
    }

    return node;
}

// Разделение переполненного дочернего узла
void split_child(BTreeNode *parent, int index, BTreeNode *child)
{
    BTreeNode *newRight = create_node(child->leaf);
    if (!newRight)
    {
        return;
    }

    // Копируем T-1 ключей из правой половины разделяемого узла
    for (int i = 0; i < MIN_KEYS; i++)
    {
        newRight->keys[i] = child->keys[T + i];
    }
    if (!child->leaf)
    {
        for (int i = 0; i < T; i++)
        {
            newRight->children[i] = child->children[T + i];
            child->children[T + i] = NULL;
        }
    }

    // Устанавливаем количество ключей для правого узла
    newRight->num_keys = MIN_KEYS;

    // Сдвигаем дочерние указатели в родителе вправо
    for (int i = parent->num_keys; i > index; i--)
    {
        parent->children[i + 1] = parent->children[i];
    }

    // Устанавливаем новый дочерний узел справа
    parent->children[index + 1] = newRight;

    // Сдвигаем ключи в родителе вправо
    for (int i = parent->num_keys - 1; i >= index; i--)
    {
        parent->keys[i + 1] = parent->keys[i];
    }

    // Поднимаем средний ключ в родителя
    parent->keys[index] = child->keys[T - 1];

    // Обновляем количество ключей
    child->num_keys = MIN_KEYS;
    parent->num_keys++;
}

// Вставка ключа в неполный узел
void insert_non_full(BTreeNode *node, int key)
{
    int i = node->num_keys - 1;

    if (node->leaf)
    {
        // Вставка в листовой узел
        while (i >= 0 && node->keys[i] > key)
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        node->keys[i + 1] = key;
        node->num_keys++;
    }
    else
    {
        // Поиск подходящего дочернего узла
        while (i >= 0 && node->keys[i] > key)
        {
            i--;
        }
        i++;

        // Проверяем, не переполнен ли дочерний узел
        if (node->children[i]->num_keys == MAX_KEYS)
        {
            split_child(node, i, node->children[i]);

            // Определяем, в какой из двух новых узлов вставлять ключ
            if (key > node->keys[i])
            {
                i++;
            }
        }

        insert_non_full(node->children[i], key);
    }
}

// Вставка ключа в B-дерево
BTreeNode *insert(BTreeNode *root, int key)
{
    if (!root)
    {
        root = create_node(true);
        root->keys[0] = key;
        root->num_keys = 1;
        return root;
    }

    // Если корень переполнен, разделяем его
    if (root->num_keys == MAX_KEYS)
    {
        BTreeNode *new_root = create_node(false);
        new_root->children[0] = root;
        split_child(new_root, 0, root);

        // Определяем, в какого ребенка вставлять ключ
        int i = 0;
        if (new_root->keys[0] < key)
        {
            i++;
        }

        insert_non_full(new_root->children[i], key);
        return new_root;
    }

    insert_non_full(root, key);
    return root;
}

// Поиск ключа в B-дереве
BTreeNode *search(BTreeNode *node, int key, int *pos)
{
    int i = 0;
    while (i < node->num_keys && key > node->keys[i]) {
        i++;
    }

    if (i < node->num_keys && key == node->keys[i]) {
        *pos = i;
        return node;
    }

    if (node->leaf) {
        return NULL;
    }

    return search(node->children[i], key, pos);
}

// Печать B-дерева (обход в порядке возрастания)
void print_tree(BTreeNode *node, int level)
{
    if (!node)
        return;

    printf("Уровень %d: ", level);
    for (int i = 0; i < node->num_keys; i++)
    {
        printf("%d ", node->keys[i]);
    }
    printf("\n");

    if (!node->leaf)
    {
        for (int i = 0; i <= node->num_keys; i++)
        {
            print_tree(node->children[i], level + 1);
        }
    }
}

// Освобождение памяти
void free_tree(BTreeNode *node)
{
    if (!node)
        return;

    if (!node->leaf)
    {
        for (int i = 0; i <= node->num_keys; i++)
        {
            free_tree(node->children[i]);
        }
    }

    free(node);
}

// Демонстрация работы
int main()
{
    BTreeNode *root = NULL;
    int pos;

    printf("B-дерево порядка %d (минимальная степень T=%d)\n", 2 * T, T);
    printf("Каждый узел может содержать от %d до %d ключей\n\n", MIN_KEYS, MAX_KEYS);

    // Вставка ключей
    int keys_to_insert[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
                            15, 25, 35, 45, 55, 65, 75, 85, 95};
    int n = sizeof(keys_to_insert) / sizeof(keys_to_insert[0]);

    printf("Вставка ключей: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", keys_to_insert[i]);
        root = insert(root, keys_to_insert[i]);
    }
    printf("\n\n");

    printf("Структура B-дерева:\n");
    print_tree(root, 0);
    printf("\n");

    // Поиск ключей
    int search_keys[] = {25, 50, 75, 100, 200};
    int search_n = sizeof(search_keys) / sizeof(search_keys[0]);

    printf("Поиск ключей:\n");
    for (int i = 0; i < search_n; i++)
    {
        BTreeNode *result = search(root, search_keys[i], &pos);
        if (result)
        {
            printf("Ключ %d найден в узле на позиции %d\n", search_keys[i], pos);
        }
        else
        {
            printf("Ключ %d не найден\n", search_keys[i]);
        }
    }

    // Освобождение памяти
    free_tree(root);

    return 0;
}