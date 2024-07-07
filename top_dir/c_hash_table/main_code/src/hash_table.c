#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash_table.h"
#include "prime.h"
#include "math.h"

static ht_item HT_DELETED_ITEM = {NULL, NULL};  // To mark an item deleted without actually deleting it.

/* Insert and delete items in the table */
static ht_item* ht_new_item(const char* k, const char* v) {
    ht_item* i = (ht_item*)malloc(sizeof(ht_item));
    if (i == NULL)
    {
        printf("MEM ALLOC FAILED for new Hash item at i = %p\n",i);
    }
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}

static void ht_del_item(ht_item* i) {
    free(i->key);
    free(i->value);
    free(i);
}

/* Create and delete hash table */
static ht_hash_table* ht_new_sized(const int base_size) {
    ht_hash_table* ht = (ht_hash_table*)malloc(sizeof(ht_hash_table)); // using malloc instead of xmalloc and logging error
    if (ht == NULL)
    {
        printf("MEM ALLOC FAILED for new Hasg Table ht = %p\n",ht);
    }
    ht->base_size = base_size;
    ht->size = next_prime(ht->base_size);
    ht->count = 0;
    ht->items = (ht_item**)calloc((size_t)ht->size, sizeof(ht_item*));
    if ( ht->items == NULL)
    {
        printf("MEM ALLOC FAILED for items at &item = %p\n",ht->items);
    }
    return ht;
}

ht_hash_table* ht_new() {
    return ht_new_sized(HT_INITIAL_BASE_SIZE);
}

void ht_del_hash_table(ht_hash_table* ht) {
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL) {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

/* Resizing a hash table */
static void ht_resize(ht_hash_table* ht, const int base_size) {
    if (base_size < HT_INITIAL_BASE_SIZE) {
        return;
    }
    ht_hash_table* new_ht = ht_new_sized(base_size);
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_insert(new_ht, item->key, item->value);
        }
    }

    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    // To delete new_ht, we give it ht's size and items 
    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;
    printf("Hash Table resized from size %d to %d\n",new_ht->size,ht->size);

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);
}

static void ht_resize_up(ht_hash_table* ht) {
    const int new_size = ht->base_size * 2;
    ht_resize(ht, new_size);
}

static void ht_resize_down(ht_hash_table* ht) {
    const int new_size = ht->base_size / 2;
    ht_resize(ht, new_size);
}

/* Hash function which uses a prime number 'a' and bucket size m to convert string s to a hash value using ASCII code of characters
* hash("cat", 151, 53) => hash = (151**2 * 99 + 151**1 * 97 + 151**0 * 116) % 53 */
static int ht_hash(const char* s, const int a, const int m) {
    long hash = 0;
    const int len_s = strlen(s);
    for (int i = 0; i < len_s; i++) {
        hash += (long)pow(a, len_s - (i+1)) * s[i];
        hash = hash % m;
    }
    return (int)hash;
}

/* FYI: Pathological inputs - Inputs with same has value causing collisions
* To handle collisions we are using "open addressing with double hashing".
* In example below the index (hash value) to be used after "i" collisions for an input is derived as:
* index = (hash_a(string) + i * (hash_b(string) + 1)) % num_buckets   */
static int ht_get_hash(const char* s, const int num_buckets, const int attempt) {
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

/* Necessary APIs */
void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    const int load = ht->count * 100 / ht->size;
    if (load > 70) {
        ht_resize_up(ht);
    }
    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item* cur_item = ht->items[index];
    int i = 1;
    while (cur_item != NULL) {    // check collision and go to next possible index
        if (cur_item != &HT_DELETED_ITEM) {   // Add in the place of a deleted item
            if (strcmp(cur_item->key, key) == 0) {
                ht_del_item(cur_item);
                ht->items[index] = item;
                return;
            }
        }
        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    } 
    ht->items[index] = item;
    ht->count++;
}

char* ht_search(ht_hash_table* ht, const char* key) {
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) { 
            if (strcmp(item->key, key) == 0) {
                return item->value;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    } 
    return NULL;
}

/* Deleting an item from hash table - Pointing to an empty item without freeing the memory */
void ht_delete(ht_hash_table* ht, const char* key) {
    const int load = ht->count * 100 / ht->size;
    if (load < 10) {
        ht_resize_down(ht);
    }
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    } 
    ht->count--;
}

int lineCount(char filename[])
{
    FILE *fp;
    int count = 0;
    char c;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Could not open file %s\n", filename);
        return 0;
    }
    // Extract characters from file and store in character c
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
    // Close the file
    fclose(fp);
    //printf("The file %s has %d lines\n", filename, count);
 
    return count;
}
int main() {
    ht_hash_table* ht = ht_new();
    FILE* fp;
    FILE* fp_result;
    char filename[MAXCHAR] = "tracefile.txt"; 
    char row[MAXCHAR];
    char *token, *command, *key, *value, *output;

    int line_count = lineCount(filename);
    fp = fopen(filename,"r");
    fp_result = fopen("result.txt","w");
    int line_num = 1;
    while (feof(fp) != TRUE)
    {
        fgets(row, MAXCHAR, fp);
        token = strtok(row, ",");
        command = token;
        if(token != NULL)
        {
#ifdef DEBUG_PRINTS
            printf("command input row = %s\n",row);
#endif
            if(!strcmp(token,"INSERT"))
            {
                token = strtok(NULL, ",");
                key = token;
                token = strtok(NULL, ",");
                value = token;
                ht_insert(ht,key,value);
                output = "PASS";
            }
            else if(!strcmp(token,"SEARCH"))
            {
                token = strtok(NULL, ",");
                key = token;
                token = strtok(NULL, ",");
                value = token;
                char* ret = ht_search(ht,key);
                if(ret != NULL)
                {
                    if(!strcmp(ret,value))
                    output = "PASS";
                    else
                    output = "FAIL";
                }
                else
                {
                    if(((line_count != line_num) && !strcmp(value,"0\n"))
                      || ((line_count == line_num) && !strcmp(value,"0\0")))
                    output = "PASS";
                    else
                    output = "FAIL";
                }
            }
            else if(!strcmp(token,"DELETE"))
            {
                token = strtok(NULL, ",");
                key = token;
                token = strtok(NULL, ",");
                value = token;
                ht_delete(ht,key);
                output = "PASS";
            }
            else if(!strcmp(token,"EOF"))
            {
                printf("Operation complete. Exiting...\n");
                fprintf(fp_result,"Operation complete.");
                break;
            }
            else
            {
                printf("ERROR: Tracefile corrupted. Exiting...\n");
                break;
            }
            char str1[MAXCHAR] ;
#ifdef DEBUG_PRINTS
            printf("write length = %d\n",fprintf(fp_result,"Line %d: %s %s\n",line_num,command,output));
#else
            fprintf(fp_result,"Line %d: %s %s\n",line_num,command,output);
#endif
            fflush(fp_result);
        }
#ifdef DEBUG_PRINTS
        printf("line = %d done.\n",line_num);
#endif
        line_num++;
    }
    fflush(fp_result);
    fclose(fp);
    fclose(fp_result);
    ht_del_hash_table(ht);
}

