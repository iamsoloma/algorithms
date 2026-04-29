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
    while (i < node->num_keys && key > node->keys[i])
    {
        i++;
    }

    if (i < node->num_keys && key == node->keys[i])
    {
        *pos = i;
        return node;
    }

    if (node->leaf)
    {
        return NULL;
    }

    return search(node->children[i], key, pos);
}

// Получение предшественника (максимальный ключ в левом поддереве)
int get_predecessor(BTreeNode *node, int index)
{
    BTreeNode *current = node->children[index];

    // Идем до самого правого листа
    while (!current->leaf)
    {
        current = current->children[current->num_keys];
    }

    // Возвращаем последний ключ
    return current->keys[current->num_keys - 1];
}

// Получение преемника (минимальный ключ в правом поддереве)
int get_successor(BTreeNode *node, int index)
{
    BTreeNode *current = node->children[index+1];

    // Спускаемся до самого левого узла
    while (!current->leaf)
    {
        current = current->children[0];
    }

    // Возвращаем наименьший ключ
    return current->keys[0];
}

// Слияние узла child с его правым братом
void merge_children(BTreeNode *parent, int index)
{
    BTreeNode *left = parent->children[index];
    BTreeNode *right = parent->children[index + 1];

    // Перемещаем ключ из parent в left
    left->keys[left->num_keys] = parent->keys[index];
    left->num_keys++;

    // Копируем все ключи из right в left
    for (int i = 0; i < right->num_keys; i++)
    {
        left->keys[left->num_keys + i] = right->keys[i];
    }

    // Копируем детей из right в left (если не листья)
    if (!left->leaf)
    {
        for (int i = 0; i <= right->num_keys; i++)
        {
            left->children[left->num_keys + i] = right->children[i];
        }
    }

    left->num_keys += right->num_keys;

    // Сдвигаем ключи в parent влево
    for (int i = index; i < parent->num_keys - 1; i++)
    {
        parent->keys[i] = parent->keys[i + 1];
    }

    // Сдвигаем детей в parent влево
    for (int i = index + 1; i < parent->num_keys; i++)
    {
        parent->children[i] = parent->children[i + 1];
    }

    parent->num_keys--;

    // Освобождаем память правого узла
    free(right);
}

// Заимствование ключа у левого брата
void borrow_from_left(BTreeNode *parent, int index)
{
    BTreeNode *child = parent->children[index];
    BTreeNode *left_sibling = parent->children[index - 1];

    // Сдвигаем ключи child вправо
    for (int i = child->num_keys - 1; i >= 0; i--)
    {
        child->keys[i + 1] = child->keys[i];
    }

    // Если не лист, сдвигаем детей
    if (!child->leaf)
    {
        for (int i = child->num_keys; i >= 0; i--)
        {
            child->children[i + 1] = child->children[i];
        }
    }

    // Перемещаем ключ из parent в child
    child->keys[0] = parent->keys[index - 1];
    child->num_keys++;

    // Перемещаем ключ из left_sibling в parent
    parent->keys[index - 1] = left_sibling->keys[left_sibling->num_keys - 1];

    // Если не листья, перемещаем ребенка
    if (!child->leaf)
    {
        child->children[0] = left_sibling->children[left_sibling->num_keys];
    }

    left_sibling->num_keys--;
}

// Заимствование ключа у правого брата
void borrow_from_right(BTreeNode *parent, int index)
{
    BTreeNode *child = parent->children[index];
    BTreeNode *right_sibling = parent->children[index + 1];

    //Перемещаем ключ из parent в child
    child->keys[child->num_keys] = parent->keys[index];
    child->num_keys++;

    //перемещаем ключ из right_sibling в parent
    parent->keys[index] = right_sibling->keys[0];

    //Сдвигаем ключи в right_sibling
    for (int i = 0; i < right_sibling->num_keys - 1; i++) {
        right_sibling->keys[i] = right_sibling->keys[i+1];
    }


    if (!child->leaf) {
        //Перемещаем первого ребёнка из right_sibling в child
        child->children[child->num_keys] = right_sibling->children[0];
        //Сдвигаем детей в right_sibling влево
        for (int i = 0; i < right_sibling->num_keys; i++) {
            right_sibling->children[i] = right_sibling->children[i+1];
        }
    }

    right_sibling->num_keys--;
}

// Вспомогательная функция удаления ключа из узла
void remove_from_node(BTreeNode *node, int key)
{
    int index = 0;

    // Находим позицию ключа
    while (index < node->num_keys && node->keys[index] < key)
    {
        index++;
    }

    // Сдвигаем ключи влево
    for (int i = index; i < node->num_keys - 1; i++)
    {
        node->keys[i] = node->keys[i + 1];
    }

    node->num_keys--;
}

// Основная функция удаления ключа
void remove_key(BTreeNode *node, int key)
{
    if (!node)
        return;

    int index = 0;

    // Находим позицию ключа (или где он должен быть)
    while (index < node->num_keys && node->keys[index] < key)
    {
        index++;
    }

    // Случай 1: Ключ найден в текущем узле
    if (index < node->num_keys && node->keys[index] == key)
    {
        if (node->leaf)
        {
            // Случай 1a: Удаляем из листа
            remove_from_node(node, key);
        }
        else
        {
            // Случай 1b: Удаляем из внутреннего узла

            // Проверяем левого ребенка
            if (node->children[index]->num_keys >= T)
            {
                int pred = get_predecessor(node, index);
                node->keys[index] = pred;
                remove_key(node->children[index], pred);
            }
            // Проверяем правого ребенка
            else if (node->children[index + 1]->num_keys >= T)
            {
                int succ = get_successor(node, index);
                node->keys[index] = succ;
                remove_key(node->children[index + 1], succ);
            }
            // Оба ребенка имеют минимальное количество ключей
            else
            {
                merge_children(node, index);
                remove_key(node->children[index], key);
            }
        }
    }
    // Случай 2: Ключ не найден, идем вниз
    else
    {
        // Если достигли листа, ключ не найден
        if (node->leaf)
        {
            printf("Ключ %d не найден в дереве\n", key);
            return;
        }

        bool is_last = (index == node->num_keys);
        BTreeNode *child = node->children[index];

        // Проверяем, нужно ли пополнить дочерний узел
        if (child->num_keys == MIN_KEYS)
        {
            // Пытаемся заимствовать у левого брата
            if (index > 0 && node->children[index - 1]->num_keys > MIN_KEYS)
            {
                borrow_from_left(node, index);
            }
            // Пытаемся заимствовать у правого брата
            else if (index < node->num_keys && node->children[index + 1]->num_keys > MIN_KEYS)
            {
                borrow_from_right(node, index);
            }
            // Объединяем с одним из братьев
            else
            {
                if (index < node->num_keys)
                {
                    merge_children(node, index);
                }
                else
                {
                    merge_children(node, index - 1);
                    child = node->children[index - 1];
                }
            }
        }

        remove_key(child, key);
    }
}

// Обертка для удаления ключа из дерева
BTreeNode *delete_key(BTreeNode *root, int key)
{
    if (!root)
        return NULL;

    remove_key(root, key);

    // Если корень стал пустым и не является листом
    if (root->num_keys == 0 && !root->leaf)
    {
        BTreeNode *new_root = root->children[0];
        free(root);
        return new_root;
    }

    return root;
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

    printf("========================================\n");
    printf("B-дерево порядка %d (минимальная степень T=%d)\n", 2 * T, T);
    printf("Каждый узел может содержать от %d до %d ключей\n", MIN_KEYS, MAX_KEYS);
    printf("========================================\n\n");

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

    printf("Структура B-дерева после вставок:\n");
    print_tree(root, 0);
    printf("\n");

    // Поиск ключей
    int search_keys[] = {25, 50, 75, 100, 200};
    int search_n = sizeof(search_keys) / sizeof(search_keys[0]);

    printf("========================================\n");
    printf("Поиск ключей:\n");
    for (int i = 0; i < search_n; i++)
    {
        BTreeNode *result = search(root, search_keys[i], &pos);
        if (result)
        {
            printf("✓ Ключ %d найден в узле на позиции %d\n", search_keys[i], pos);
        }
        else
        {
            printf("✗ Ключ %d не найден\n", search_keys[i]);
        }
    }
    printf("\n");

    // Удаление ключей
    printf("========================================\n");
    printf("Удаление ключей:\n");
    int delete_keys[] = {30, 50, 20, 70, 100, 25, 40, 15};
    int delete_n = sizeof(delete_keys) / sizeof(delete_keys[0]);

    for (int i = 0; i < delete_n; i++)
    {
        printf("\n--- Удаление ключа %d ---\n", delete_keys[i]);
        root = delete_key(root, delete_keys[i]);
        printf("Дерево после удаления:\n");
        print_tree(root, 0);
        printf("\n");
    }

    // Финальная проверка поиска
    printf("========================================\n");
    printf("Финальная проверка поиска:\n");
    int final_check[] = {30, 50, 20, 70, 100, 25, 40, 15, 80, 90};
    for (int i = 0; i < 10; i++)
    {
        BTreeNode *result = search(root, final_check[i], &pos);
        if (result)
        {
            printf("✓ Ключ %d найден\n", final_check[i]);
        }
        else
        {
            printf("✗ Ключ %d не найден\n", final_check[i]);
        }
    }

    // Освобождение памяти
    free_tree(root);

    printf("\n========================================\n");
    printf("Программа завершена\n");

    return 0;
}