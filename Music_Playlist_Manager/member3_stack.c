/**
 * ============================================================================
 * Member 3: Stack (Recently Played History) Implementation
 * File: member3_stack.c
 * 
 * Description:
 * Manages the playback history using a Stack implemented via a Linked List.
 * Follows LIFO (Last In, First Out). Songs are pushed when marked as completed.
 * Allows displaying history from top to bottom without modifying the stack,
 * popping the most recent entry, and safe cleanup.
 * ============================================================================
 */

#include "common.h"

/**
 * Initializes an empty Stack.
 */
void initStack(Stack* s) {
    if (s == NULL) return;
    s->top = NULL;
    s->count = 0;
}

/**
 * Checks whether the Stack is empty.
 * Returns 1 if empty, 0 otherwise.
 */
int isStackEmpty(const Stack* s) {
    return (s == NULL || s->top == NULL);
}

/**
 * Pushes a completed song onto the top of the Stack.
 * Follows LIFO principle.
 */
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

/**
 * Pops and returns the most recently completed song from the top of the Stack.
 * Returns pointer to the Song, or NULL if stack is empty.
 */
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

/**
 * Displays playback history from TOP (most recent) to BOTTOM without popping.
 */
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

/**
 * Removes any entries referencing a specific song ID from the Stack.
 * Called when a song is deleted from the playlist to prevent dangling pointers.
 */
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

/**
 * Frees all dynamically allocated memory for Stack nodes.
 */
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

