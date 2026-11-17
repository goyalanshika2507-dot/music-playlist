/**
 * ============================================================================
 * Member 4: Hashing Implementation (Search Song)
 * File: member4_hashing.c
 * 
 * Description:
 * Implements a Hash Table using Separate Chaining (linked lists) to resolve collisions.
 * Provides O(1) average-time complexity for searching songs by Song ID.
 * Stores pointers/references to the master Song nodes in the Doubly Linked List,
 * eliminating redundant memory usage while keeping indexing consistent.
 * 
 * Hash Function:
 *   hashIndex = songID % TABLE_SIZE;
 * Collisions are handled by appending or prepending to a linked list at that bucket.
 * ============================================================================
 */

#include "common.h"

/**
 * Initializes the Hash Table buckets to NULL.
 */
void initHashTable(HashTable* ht) {
    if (ht == NULL) return;
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->table[i] = NULL;
    }
}

/**
 * Simple modulo hash function.
 * Maps any positive integer Song ID into the range [0, TABLE_SIZE - 1].
 */
int hashFunction(int songId) {
    int index = songId % TABLE_SIZE;
    if (index < 0) {
        index += TABLE_SIZE; /* Guard against negative IDs if any */
    }
    return index;
}

/**
 * Inserts a reference/pointer to an existing Song node into the Hash Table.
 * Uses Separate Chaining: if multiple songs map to the same bucket index,
 * they are chained together in a singly linked list.
 */
void insertIntoHashTable(HashTable* ht, Song* song) {
    if (ht == NULL || song == NULL) return;

    int index = hashFunction(song->id);

    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    if (newNode == NULL) {
        printf("Error: Memory allocation failed for Hash Node!\n");
        return;
    }

    newNode->song = song;
    /* Insert at the beginning of the chain at bucket index */
    newNode->next = ht->table[index];
    ht->table[index] = newNode;
}

/**
 * Searches for a song by Song ID in the Hash Table.
 * Calculates hash index in O(1) and traverses only the collision chain for that bucket.
 * 
 * Returns pointer to the Song if found, or NULL if not found.
 */
Song* searchHashTable(const HashTable* ht, int songId) {
    if (ht == NULL) return NULL;

    int index = hashFunction(songId);
    HashNode* curr = ht->table[index];

    /* Traverse the collision chain at bucket[index] */
    while (curr != NULL) {
        if (curr->song != NULL && curr->song->id == songId) {
            return curr->song; /* Found */
        }
        curr = curr->next;
    }

    return NULL; /* Unsuccessful search */
}

/**
 * Deletes a song's reference from the Hash Table by Song ID.
 * Returns 1 if found and deleted, 0 if not found.
 */
int deleteFromHashTable(HashTable* ht, int songId) {
    if (ht == NULL) return 0;

    int index = hashFunction(songId);
    HashNode* curr = ht->table[index];
    HashNode* prev = NULL;

    while (curr != NULL) {
        if (curr->song != NULL && curr->song->id == songId) {
            if (prev == NULL) {
                /* Deleting first node in the bucket chain */
                ht->table[index] = curr->next;
            } else {
                /* Deleting subsequent node in the chain */
                prev->next = curr->next;
            }
            free(curr);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }

    return 0; /* Not found in hash table */
}

/**
 * Displays the internal Hash Table buckets and collision chains.
 * Very helpful for viva demonstration of how separate chaining works!
 */
void displayHashTable(const HashTable* ht) {
    if (ht == NULL) return;

    printf("\n======================= HASH TABLE STATUS (Size: %d) =======================\n", TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Bucket [%2d]: ", i);
        HashNode* curr = ht->table[i];
        if (curr == NULL) {
            printf("--- (Empty)\n");
        } else {
            while (curr != NULL) {
                if (curr->song != NULL) {
                    printf("[ID: %d | \"%s\"] -> ", curr->song->id, curr->song->title);
                }
                curr = curr->next;
            }
            printf("NULL\n");
        }
    }
    printf("============================================================================\n");
}

/**
 * Frees all memory allocated for Hash Table collision nodes.
 * Note: Does not free the Song pointers themselves (they are owned by DLL).
 */
void freeHashTable(HashTable* ht) {
    if (ht == NULL) return;
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* curr = ht->table[i];
        while (curr != NULL) {
            HashNode* next = curr->next;
            free(curr);
            curr = next;
        }
        ht->table[i] = NULL;
    }
}

