#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hashMap.h"

/* struct hashLink {
   KeyType key; //the key is what you use to look up a hashLink
   ValueType value; //the value stored with the hashLink, a pointer to int in the case of concordance
   struct hashLink * next; //notice how these are like linked list nodes
};
typedef struct hashLink hashLink;
*/

/*
struct hashMap {
    hashLink ** table; //array of pointers to hashLinks
    int tableSize; //number of buckets in the table
    int count; //number of hashLinks in the table
};
typedef struct hashMap hashMap;
*/

/*the first hashing function you can use*/
int stringHash1(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += str[i];
	return r;
}

/*the second hashing function you can use*/
int stringHash2(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += (i+1) * str[i]; /*the difference between stringHash1 and stringHash2 is on this line*/
	return r;
}

/* initialize the supplied hashMap struct*/
void _initMap (struct hashMap * ht, int tableSize)
{
	int index;
	if(ht == NULL)
		return;
	ht->table = (hashLink**)malloc(sizeof(hashLink*) * tableSize);
	ht->tableSize = tableSize;
	ht->count = 0;
	for(index = 0; index < tableSize; index++)
		ht->table[index] = NULL;
}

/* allocate memory and initialize a hash map*/
hashMap *createMap(int tableSize) {
	assert(tableSize > 0);
	hashMap *ht;
	ht = malloc(sizeof(hashMap));
	assert(ht != 0);
	_initMap(ht, tableSize);
	return ht;
}

/*
 Free all memory used by the buckets.
 Note: Before freeing up a hashLink, free the memory occupied by key and value
 */
void _freeMap (struct hashMap * ht)
{
    struct hashLink* next;
    struct hashLink* cur;
    for (int i=0; i < ht->tableSize; i++){
        cur = ht->table[i];
        while (cur != 0) {
            next = cur->next;
            free(cur);
            cur = next;
        }
        ht->table[i] = 0;
    }
    free(ht->table);
    ht->tableSize = 0;
    ht->count = 0;
}

/* Deallocate buckets and the hash map.*/
void deleteMap(hashMap *ht) {
	assert(ht!= 0);
	/* Free all memory used by the buckets */
	_freeMap(ht);
	/* free the hashMap struct */
	free(ht);
}

/*
Resizes the hash table to be the size newTableSize
*/
void _setTableSize(struct hashMap * ht, int newTableSize)
{
    struct hashLink **oldTable = ht->table;
    int oldTableSize = capacity(ht);
    hashLink *cur;
    hashLink *prev;
    _initMap(ht, newTableSize);
    for (int i=0; i < oldTableSize; i++) {
        cur = oldTable[i];
        while (cur != 0){
            insertMap(ht, cur->key, cur->value);
            prev = cur;
            cur = cur->next;
            free(prev);
        }
    }
    free(oldTable);
}

/*
 insert the following values into a hashLink, you must create this hashLink but
 only after you confirm that this key does not already exist in the table. For example, you
 cannot have two hashLinks for the word "taco".

 if a hashLink already exists in the table for the key provided you should
 replace that hashLink--this requires freeing up the old memory pointed by hashLink->value
 and then pointing hashLink->value to value v.

 also, you must monitor the load factor and resize when the load factor is greater than
 or equal LOAD_FACTOR_THRESHOLD (defined in hashMap.h).
 */
void insertMap (struct hashMap * ht, KeyType k, ValueType v)
{
    int idx;
    if (HASHING_FUNCTION == 1){
        idx = stringHash1(k) % ht->tableSize;
    } else {
        idx = stringHash2(k) % ht->tableSize;
    }
    if (idx < 0){
        idx += ht->tableSize;
    }
    struct hashLink* new = malloc(sizeof(struct hashLink));
    assert(new);

    new->next = 0;
    new->key = k;
    new->value = v;
    if (containsKey(ht, k)){
        removeKey(ht, k);
    }
    if (ht->table[idx]){
        new->next = ht->table[idx];
    }
    ht->table[idx] = new;
    ht->count++;
    if (tableLoad(ht) >= LOAD_FACTOR_THRESHOLD){
        _setTableSize(ht, 2 * capacity(ht));
    }
}

/*
 this returns the value (which is void*) stored in a hashLink specified by the key k.

 if the user supplies the key "taco" you should find taco in the hashTable, then
 return the value member of the hashLink that represents taco.

 if the supplied key is not in the hashtable return NULL.
 */
ValueType* atMap (struct hashMap * ht, KeyType k)
{
    int idx;
    if (HASHING_FUNCTION == 1){
        idx = stringHash1(k) % ht->tableSize;
    } else {
        idx = stringHash2(k) % ht->tableSize;
    }
    if (idx < 0){
        idx += ht->tableSize;
    }
    assert(containsKey(ht, k));
    struct hashLink* cur = ht->table[idx];
    while (strcmp(cur->key, k) != 0){
        cur = cur->next;
    }
    return &cur->value;
}

/*
 a simple yes/no if the key is in the hashtable.
 0 is no, all other values are yes.
 */
int containsKey (struct hashMap * ht, KeyType k)
{
    int idx;
    if (HASHING_FUNCTION == 1){
        idx = stringHash1(k) % ht->tableSize;
    } else {
        idx = stringHash2(k) % ht->tableSize;
    }
    if (idx < 0){
        idx += ht->tableSize;
    }
    struct hashLink* cur = ht->table[idx];
    while (cur){
        if (strcmp(cur->key, k) == 0){
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

/*
 find the hashlink for the supplied key and remove it, also freeing the memory
 for that hashlink. it is not an error to be unable to find the hashlink, if it
 cannot be found do nothing (or print a message) but do not use an assert which
 will end your program.
 */
void removeKey (struct hashMap * ht, KeyType k)
{
    struct hashLink* cur;
    struct hashLink* pre;
    int idx;
    if (HASHING_FUNCTION == 1){
        idx = stringHash1(k) % ht->tableSize;
    } else {
        idx = stringHash2(k) % ht->tableSize;
    }
    if (idx < 0){
        idx += ht->tableSize;
    }
    if (containsKey(ht, k)){
        cur = ht->table[idx];
        pre = ht->table[idx];

        while (cur != 0){
            if (strcmp(cur->key, k) == 0){
                if (cur == ht->table[idx]){
                    ht->table[idx] = cur->next;
                } else{
                    pre->next = cur->next;
                }
                free(cur);
                cur = 0;
            } else {
                pre = cur;
                cur = cur->next;
            }
        }
    }
}

/*
 returns the number of hashLinks in the table
 */
int size (struct hashMap *ht)
{
    return ht->count;
}

/*
 returns the number of buckets in the table
 */
int capacity(struct hashMap *ht)
{
    assert(ht);
    return ht->tableSize;
}

/*
 returns the number of empty buckets in the table, these are buckets which have
 no hashlinks hanging off of them.
 */
int emptyBuckets(struct hashMap *ht)
{
    int numEmptyBuckets = 0;
    for (int i= 0; i < ht->tableSize; i++){
        if (!ht->table[i]){
            numEmptyBuckets++;
        }
    }
    return numEmptyBuckets;
}

/*
 returns the ratio of: (number of hashlinks) / (number of buckets)

 this value can range anywhere from zero (an empty table) to more then 1, which
 would mean that there are more hashlinks then buckets (but remember hashlinks
 are like linked list nodes so they can hang from each other)
 */
float tableLoad(struct hashMap *ht)
{
    assert(ht);
    return ht->count / (double) ht->tableSize;
}
void printMap (struct hashMap * ht)
{
    int i;
	struct hashLink *temp;
	for(i = 0;i < capacity(ht); i++){
		temp = ht->table[i];
		if(temp != 0) {
			printf("\nBucket Index %d -> ", i);
		}
		while(temp != 0){
			printf("Key:%s|", temp->key);
			printValue(temp->value);
			printf(" -> ");
			temp=temp->next;
		}
	}
}


