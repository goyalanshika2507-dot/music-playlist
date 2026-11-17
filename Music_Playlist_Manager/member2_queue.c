/**
 * ============================================================================
 * Member 2: Queue Implementation
 * File: member2_queue.c
 * 
 * Description:
 * Manages the play queue using a Linked List following FIFO (First In, First Out).
 * Allows users to enqueue songs from the main playlist, play queued songs in order,
 * display the queue, and automatically cleans up references if a song is deleted.
 * ============================================================================
 */

#include "common.h"

/**
 * Initializes an empty Queue.
 */
void initQueue(Queue* q) {
    if (q == NULL) return;
    q->front = NULL;
    q->rear = NULL;
    q->count = 0;
}

/**
 * Checks whether the Queue is empty.
 * Returns 1 if empty, 0 otherwise.
 */
int isQueueEmpty(const Queue* q) {
    return (q == NULL || q->front == NULL);
}

/**
 * Enqueues a song reference at the rear of the Queue.
 * Follows FIFO principle.
 */
void enqueueSong(Queue* q, Song* song) {
    if (q == NULL || song == NULL) return;

    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        printf("Error: Memory allocation failed for Queue Node!\n");
        return;
    }

    newNode->song = song;
    newNode->next = NULL;

    if (q->rear == NULL) {
        /* Queue is currently empty */
        q->front = newNode;
        q->rear = newNode;
    } else {
        /* Append at the rear */
        q->rear->next = newNode;
        q->rear = newNode;
    }

    q->count++;
    printf(">> '%s' added to play queue (Position: #%d).\n", song->title, q->count);
}

/**
 * Dequeues and returns the song from the front of the Queue.
 * Returns pointer to the Song, or NULL if queue is empty.
 */
Song* dequeueSong(Queue* q) {
    if (isQueueEmpty(q)) {
        return NULL;
    }

    QueueNode* temp = q->front;
    Song* song = temp->song;

    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    q->count--;
    return song;
}

/**
 * Displays all songs currently in the Queue from front to rear.
 */
void displayQueue(const Queue* q) {
    if (isQueueEmpty(q)) {
        printf("\n>>> Play Queue is empty. No upcoming songs! <<<\n");
        return;
    }

    printf("\n============================ UPCOMING PLAY QUEUE (%d) ============================\n", q->count);
    printf("%-5s | %-6s | %-28s | %-22s | %s\n",
           "Pos", "ID", "Title", "Artist", "Duration");
    printf("-----------------------------------------------------------------------------------\n");

    const QueueNode* curr = q->front;
    int pos = 1;
    while (curr != NULL) {
        if (curr->song != NULL) {
            printf("#%-4d | %-6d | %-28s | %-22s | %5.2f mins\n",
                   pos, curr->song->id, curr->song->title, curr->song->artist, curr->song->duration);
        }
        curr = curr->next;
        pos++;
    }
    printf("===================================================================================\n");
}

/**
 * Removes any queued entries referencing a specific song ID.
 * Called when a song is deleted from the playlist to prevent dangling pointers.
 */
void removeSongFromQueue(Queue* q, int songId) {
    if (q == NULL || q->front == NULL) return;

    QueueNode* curr = q->front;
    QueueNode* prev = NULL;

    while (curr != NULL) {
        if (curr->song != NULL && curr->song->id == songId) {
            QueueNode* toDelete = curr;
            if (prev == NULL) {
                /* Deleting front node */
                q->front = curr->next;
                curr = q->front;
            } else {
                prev->next = curr->next;
                curr = curr->next;
            }

            /* Adjust rear if last node was removed */
            if (toDelete == q->rear) {
                q->rear = prev;
            }

            free(toDelete);
            q->count--;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

/**
 * Frees all dynamically allocated memory for Queue nodes.
 */
void freeQueue(Queue* q) {
    if (q == NULL) return;
    QueueNode* curr = q->front;
    while (curr != NULL) {
        QueueNode* next = curr->next;
        free(curr);
        curr = next;
    }
    q->front = NULL;
    q->rear = NULL;
    q->count = 0;
}

