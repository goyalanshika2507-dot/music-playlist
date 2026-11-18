#include "common.h"

void initQueue(Queue* q) {
    if (q == NULL) return;
    q->front = NULL;
    q->rear = NULL;
    q->count = 0;
}

int isQueueEmpty(const Queue* q) {
    return (q == NULL || q->front == NULL);
}

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
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }

    q->count++;
    printf(">> '%s' added to play queue (Position: #%d).\n", song->title, q->count);
}

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

void removeSongFromQueue(Queue* q, int songId) {
    if (q == NULL || q->front == NULL) return;

    QueueNode* curr = q->front;
    QueueNode* prev = NULL;

    while (curr != NULL) {
        if (curr->song != NULL && curr->song->id == songId) {
            QueueNode* toDelete = curr;
            if (prev == NULL) {
                q->front = curr->next;
                curr = q->front;
            } else {
                prev->next = curr->next;
                curr = curr->next;
            }
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

