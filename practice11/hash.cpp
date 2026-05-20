#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LEN 100

// Структура узла для цепочки
typedef struct Node {
    char word[MAX_WORD_LEN];
    struct Node* next;
} Node;

// Структура хеш-таблицы
typedef struct {
    Node** buckets;     // массив указателей на начала цепочек
    int size;           // размер таблицы
    int (*hash_func)(const char*, int);  // указатель на хеш-функцию
} HashTable;

// ==================== ХЕШ-ФУНКЦИИ ====================

// 1. Простая сумма кодов символов
int hash_simple_sum(const char* word, int m) {
    //hash = sum(ord(c) for c in word) % M
    unsigned int hash = 0;
    for (int i = 0; word[i] != '\0'; i++) {
        hash += (unsigned char)word[i];
    }
    return hash % m;
}

// 2. Взвешенная сумма (с учётом позиции символа)
int hash_weighted_sum(const char* word, int m) {
    //hash = sum(ord(c) * (i+1) for i,c in enumerate(word)) % M
    unsigned int hash = 0;
    for (int i = 0; word[i] != '\0'; i++) {
        hash += ((unsigned char)word[i] * (i+1));
    } 
    return hash % m;
}

// 3. Полиномиальная хеш-функция (константа 31)
int hash_polynomial(const char* word, int m) {
    //	hash = (hash * 31 + ord(c)) % M
    unsigned int hash = 0;
    for (int i = 0; word[i] != '\0'; i++) {
        hash = (hash * 31 + (unsigned char)word[i]);// % m;
    }
    return hash %m;
}

// 4. XOR-хеш
int hash_xor(const char* word, int m) {
    //hash = hash ^ ord(c) затем % M
    unsigned int hash = 0;
    for (int i = 0; word[i] != '\0'; i++) {
        hash ^= (unsigned char)word[i];
    }
    return hash % m;
}

// 5. Длина + ASCII первого символа
int hash_len_first(const char* word, int m) {
    //hash = (len(word) * 100 + ord(word[0])) % M
    unsigned int hash = 0;
    int lenght = strlen(word);
    hash = lenght * 100 + (unsigned char)word[0];
    return hash % m;
}

// ==================== ОПЕРАЦИИ С ТАБЛИЦЕЙ ====================

// Создание хеш-таблицы
HashTable* create_table(int size, int (*hash_func)(const char*, int)) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    ht->size = size;
    ht->hash_func = hash_func;
    ht->buckets = (Node**)calloc(size, sizeof(Node*));
    return ht;
}

// Вставка слова (без дубликатов)
void insert(HashTable* ht, const char* word) {
    int index = ht->hash_func(word, ht->size);
    
    // Проверка на существование
    Node* current = ht->buckets[index];
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) return;
        current = current->next;
    }
    
    // Вставка в начало цепочки
    Node* new_node = (Node*)malloc(sizeof(Node));
    strcpy(new_node->word, word);
    new_node->next = ht->buckets[index];
    ht->buckets[index] = new_node;
}

// Поиск слова (возвращает 1 если найдено, 0 если нет)
// Через указатель comparisons возвращает количество сравнений
int search(HashTable* ht, const char* word, int* comparisons) {
    int index = ht->hash_func(word, ht->size);
    *comparisons = 0;
    
    Node* current = ht->buckets[index];
    while (current != NULL) {
        (*comparisons)++;
        if (strcmp(current->word, word) == 0) {
            return 1; // найдено
        }
        current = current->next;
    }
    return 0; // не найдено
}

// Удаление всех слов, начинающихся на заданную букву
int delete_by_first_letter(HashTable* ht, char letter) {
    int deleted = 0;
    
    for (int i = 0; i < ht->size; i++) {
        Node** current = &(ht->buckets[i]);
        
        while (*current != NULL) {
            if ((*current)->word[0] == letter || 
                (*current)->word[0] == letter - 32 || // для uppercase
                (*current)->word[0] == letter + 32) { // для lowercase
                Node* to_delete = *current;
                *current = (*current)->next;
                free(to_delete);
                deleted++;
            } else {
                current = &((*current)->next);
            }
        }
    }
    return deleted;
}

// Вывод таблицы (только непустые ячейки)
void display(HashTable* ht) {
    printf("\nХеш-таблица (размер %d):\n", ht->size);
    printf("========================================\n");
    
    for (int i = 0; i < ht->size; i++) {
        if (ht->buckets[i] != NULL) {
            printf("[%3d] ", i);
            Node* current = ht->buckets[i];
            while (current != NULL) {
                printf("%s", current->word);
                if (current->next) printf(" -> ");
                current = current->next;
            }
            printf("\n");
        }
    }
    printf("========================================\n");
}

// Освобождение памяти
void free_table(HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        Node* current = ht->buckets[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht->buckets);
    free(ht);
}

// ==================== ДЕМОНСТРАЦИЯ ====================

int main() {
    // Тестовые данные
    char* words[] = {"apple", "banana", "cherry", "stop", "pots", "tops", "spot", "post"};
    int word_count = 8;
    
    // Демонстрация разных хеш-функций
    
    printf("=== ДЕМОНСТРАЦИЯ ХЕШ-ФУНКЦИЙ ===\n\n");
    
    // 1. Простая сумма
    printf("1. Хеш-функция: ПРОСТАЯ СУММА (M=10)\n");
    HashTable* ht1 = create_table(10, hash_simple_sum);
    for (int i = 0; i < word_count; i++) insert(ht1, words[i]);
    display(ht1);
    
    // Поиск слова
    char* search_word = "stop";
    int comparisons;
    if (search(ht1, search_word, &comparisons)) {
        printf("Слово '%s' найдено! Сравнений: %d\n", search_word, comparisons);
    } else {
        printf("Слово '%s' не найдено\n", search_word);
    }
    
    // Удаление слов на букву 's'
    int deleted = delete_by_first_letter(ht1, 's');
    printf("\nУдалено слов на букву 's': %d\n", deleted);
    display(ht1);
    free_table(ht1);
    
    // 2. Взвешенная сумма
    printf("\n\n2. Хеш-функция: ВЗВЕШЕННАЯ СУММА (M=10)\n");
    HashTable* ht2 = create_table(10, hash_weighted_sum);
    for (int i = 0; i < word_count; i++) insert(ht2, words[i]);
    display(ht2);
    
    search_word = "apple";
    if (search(ht2, search_word, &comparisons)) {
        printf("Слово '%s' найдено! Сравнений: %d\n", search_word, comparisons);
    }
    free_table(ht2);
    
    // 3. Полиномиальная
    printf("\n\n3. Хеш-функция: ПОЛИНОМИАЛЬНАЯ (M=10)\n");
    HashTable* ht3 = create_table(10, hash_polynomial);
    for (int i = 0; i < word_count; i++) insert(ht3, words[i]);
    display(ht3);
    free_table(ht3);
    
    // 4. XOR
    printf("\n\n4. Хеш-функция: XOR (M=10)\n");
    HashTable* ht4 = create_table(10, hash_xor);
    for (int i = 0; i < word_count; i++) insert(ht4, words[i]);
    display(ht4);
    free_table(ht4);
    
    // 5. Длина + первый символ
    printf("\n\n5. Хеш-функция: ДЛИНА+ПЕРВЫЙ (M=10)\n");
    HashTable* ht5 = create_table(10, hash_len_first);
    for (int i = 0; i < word_count; i++) insert(ht5, words[i]);
    display(ht5);
    free_table(ht5);
    
    return 0;
}