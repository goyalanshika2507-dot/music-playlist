#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================================================================= */
/*                              CONSTANTS                                    */
/* ========================================================================= */
#define TABLE_SIZE 10      /* Size of the Hash Table for Song ID lookup     */
#define STRING_MAX 100     /* Maximum string length for Title and Artist     */

/* ========================================================================= */
/*                          SHARED DATA STRUCTURES                           */
/* ========================================================================= */

/**
 * Structure representing a Song (also acts as a node in the Doubly Linked List).
 * Managed primarily by Member 1 (Doubly Linked List).
 */
typedef struct Song {
    int id;                     /* Unique Song ID                            */
    char title[STRING_MAX];     /* Song Title                                */
    char artist[STRING_MAX];    /* Singer / Artist Name                      */
    float duration;             /* Song duration in minutes (e.g. 3.45)      */
    struct Song* prev;          /* Pointer to the previous song in playlist  */
    struct Song* next;          /* Pointer to the next song in playlist      */
} Song;

/**
 * Playlist structure to keep track of the head and tail of the Doubly Linked List.
 */
typedef struct Playlist {
    Song* head;                 /* Pointer to first song in the playlist     */
    Song* tail;                 /* Pointer to last song in the playlist      */
    int count;                  /* Total number of songs in the playlist     */
} Playlist;

/**
 * Node for the Queue (stores a reference/pointer to an existing Song).
 * Managed by Member 2 (Queue).
 */
typedef struct QueueNode {
    Song* song;                 /* Pointer to the song in the playlist       */
    struct QueueNode* next;     /* Pointer to the next queued item           */
} QueueNode;

/**
 * Queue structure implementing FIFO (First In, First Out).
 */
typedef struct Queue {
    QueueNode* front;           /* Pointer to the front of the queue         */
    QueueNode* rear;            /* Pointer to the rear of the queue          */
    int count;                  /* Number of songs in the queue              */
} Queue;

/**
 * Node for the Stack (stores a reference/pointer to an existing Song).
 * Managed by Member 3 (Stack).
 */
typedef struct StackNode {
    Song* song;                 /* Pointer to the completed song             */
    struct StackNode* next;     /* Pointer to the next item below in stack   */
} StackNode;

/**
 * Stack structure implementing LIFO (Last In, First Out) for playback history.
 */
typedef struct Stack {
    StackNode* top;             /* Pointer to the top of the history stack   */
    int count;                  /* Number of songs in history                */
} Stack;

/**
 * Node for Hash Table Separate Chaining (stores a reference to an existing Song).
 * Managed by Member 4 (Hashing).
 */
typedef struct HashNode {
    Song* song;                 /* Pointer to the song stored in DLL         */
    struct HashNode* next;      /* Pointer to next node in collision chain   */
} HashNode;

/**
 * Hash Table structure with separate chaining for collision resolution.
 */
typedef struct HashTable {
    HashNode* table[TABLE_SIZE]; /* Array of pointers to bucket head nodes   */
} HashTable;

/* ========================================================================= */
/*                         FUNCTION PROTOTYPES                               */
/* ========================================================================= */

/* --- Member 1: Doubly Linked List (Playlist) Operations --- */
void initPlaylist(Playlist* pl);
Song* createSong(int id, const char* title, const char* artist, float duration);
int addSongToPlaylist(Playlist* pl, Song* newSong);
int removeSongFromPlaylist(Playlist* pl, int id, Song** currentSong);
void displayPlaylist(const Playlist* pl, const Song* currentSong);
void displaySongCard(const Song* song);
void freePlaylist(Playlist* pl);

/* --- Member 2: Queue Operations --- */
void initQueue(Queue* q);
int isQueueEmpty(const Queue* q);
void enqueueSong(Queue* q, Song* song);
Song* dequeueSong(Queue* q);
void displayQueue(const Queue* q);
void removeSongFromQueue(Queue* q, int songId);
void freeQueue(Queue* q);

/* --- Member 3: Stack (Recently Played History) Operations --- */
void initStack(Stack* s);
int isStackEmpty(const Stack* s);
void pushHistory(Stack* s, Song* song);
Song* popHistory(Stack* s);
void displayHistory(const Stack* s);
void removeSongFromStack(Stack* s, int songId);
void freeStack(Stack* s);

/* --- Member 4: Hashing Operations --- */
void initHashTable(HashTable* ht);
int hashFunction(int songId);
void insertIntoHashTable(HashTable* ht, Song* song);
Song* searchHashTable(const HashTable* ht, int songId);
int deleteFromHashTable(HashTable* ht, int songId);
void displayHashTable(const HashTable* ht);
void freeHashTable(HashTable* ht);

/* --- Helper Utilities --- */
void clearInputBuffer(void);
void readString(const char* prompt, char* buffer, int maxLen);
int readInteger(const char* prompt);
float readFloat(const char* prompt);

#endif /* COMMON_H */

