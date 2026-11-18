#include "common.h"

void initStack(Stack* s) {
    if (s == NULL) return;
    s->top = NULL;
    s->count = 0;
}

int isStackEmpty(const Stack* s) {
    return (s == NULL || s->top == NULL);
}

void pushHistory(Stack* s, Song* song) {
    if (s == NULL || song == NULL) return;

    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (newNode == NULL) {
        printf("Error: Memory allocation failed for Stack Node!\n");
        return;
    }

    newNode->song = song;
    newNode->next = s->top;
    s->top = newNode;
    s->count++;

    printf(">> Logged '%s' into Recently Played History.\n", song->title);
}

Song* popHistory(Stack* s) {
    if (isStackEmpty(s)) {
        return NULL;
    }

    StackNode* temp = s->top;
    Song* song = temp->song;

    s->top = s->top->next;
    free(temp);
    s->count--;
    return song;
}

void displayHistory(const Stack* s) {
    if (isStackEmpty(s)) {
        printf("\n>>> Playback history is empty. No songs have completed playback yet! <<<\n");
        return;
    }

    printf("\n======================= RECENTLY PLAYED HISTORY (LIFO: %d) =======================\n", s->count);
    printf("%-6s | %-6s | %-28s | %-22s | %s\n",
           "Rec#", "ID", "Title", "Artist", "Duration");
    printf("-----------------------------------------------------------------------------------\n");

    const StackNode* curr = s->top;
    int index = 1;
    while (curr != NULL) {
        if (curr->song != NULL) {
            printf("#%-5d | %-6d | %-28s | %-22s | %5.2f mins\n",
                   index, curr->song->id, curr->song->title, curr->song->artist, curr->song->duration);
        }
        curr = curr->next;
        index++;
    }
    printf("===================================================================================\n");
}

void removeSongFromStack(Stack* s, int songId) {
    if (s == NULL || s->top == NULL) return;

    StackNode* curr = s->top;
    StackNode* prev = NULL;

    while (curr != NULL) {
        if (curr->song != NULL && curr->song->id == songId) {
            StackNode* toDelete = curr;
            if (prev == NULL) {
                s->top = curr->next;
                curr = s->top;
            } else {
                prev->next = curr->next;
                curr = curr->next;
            }
            free(toDelete);
            s->count--;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

void freeStack(Stack* s) {
    if (s == NULL) return;
    StackNode* curr = s->top;
    while (curr != NULL) {
        StackNode* next = curr->next;
        free(curr);
        curr = next;
    }
    s->top = NULL;
    s->count = 0;
}

