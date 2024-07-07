#define HT_PRIME_1 131
#define HT_PRIME_2 137 
#define HT_INITIAL_BASE_SIZE 7 
#define MAXCHAR 1024

#define TRUE 1
#define FALSE 0

typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct {
    int size;
    int count;
    int base_size;
    ht_item** items;
} ht_hash_table;

/* Necessary APIs */
void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* h, const char* key);