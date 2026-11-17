/**
 * ============================================================================
 * Member 1: Doubly Linked List (DLL) Implementation
 * File: member1_dll.c
 * 
 * Description:
 * Manages the main music playlist using a Doubly Linked List.
 * Provides functions to add, remove, display, and navigate songs (next/prev),
 * with careful handling of first, middle, and last node deletions.
 * ============================================================================
 */

#include "common.h"

/**
 * Initializes an empty playlist.
 */
void initPlaylist(Playlist* pl) {
    if (pl == NULL) return;
    pl->head = NULL;
    pl->tail = NULL;
    pl->count = 0;
}

/**
 * Dynamically creates a new Song node using malloc().
 * Returns pointer to the new Song, or NULL if memory allocation fails.
 */
Song* createSong(int id, const char* title, const char* artist, float duration) {
    Song* newSong = (Song*)malloc(sizeof(Song));
    if (newSong == NULL) {
        printf("Error: Memory allocation failed for new song!\n");
        return NULL;
    }
    newSong->id = id;
    strncpy(newSong->title, title, STRING_MAX - 1);
    newSong->title[STRING_MAX - 1] = '\0';
    
    strncpy(newSong->artist, artist, STRING_MAX - 1);
    newSong->artist[STRING_MAX - 1] = '\0';
    
    newSong->duration = duration;
    newSong->prev = NULL;
    newSong->next = NULL;
    return newSong;
}

/**
 * Adds a song node to the end of the Doubly Linked List.
 * Returns 1 on success, 0 on failure.
 */
int addSongToPlaylist(Playlist* pl, Song* newSong) {
    if (pl == NULL || newSong == NULL) {
        return 0;
    }

    /* Case 1: Empty playlist - new node becomes both head and tail */
    if (pl->head == NULL) {
        pl->head = newSong;
        pl->tail = newSong;
        newSong->prev = NULL;
        newSong->next = NULL;
    } else {
        /* Case 2: Append at the end (tail) */
        pl->tail->next = newSong;
        newSong->prev = pl->tail;
        newSong->next = NULL;
        pl->tail = newSong;
    }

    pl->count++;
    return 1;
}

/**
 * Removes a song from the Doubly Linked List by Song ID.
 * Handles deletion of first, middle, and last nodes properly.
 * Also adjusts *currentSong if the currently playing song is deleted.
 * 
 * Returns 1 if found and removed, 0 if not found.
 */
int removeSongFromPlaylist(Playlist* pl, int id, Song** currentSong) {
    if (pl == NULL || pl->head == NULL) {
        return 0; /* Playlist is empty */
    }

    Song* curr = pl->head;

    /* Search for the song node with matching ID */
    while (curr != NULL && curr->id != id) {
        curr = curr->next;
    }

    /* If song not found */
    if (curr == NULL) {
        return 0;
    }

    /* Adjust currentSong pointer if the playing song is being deleted */
    if (currentSong != NULL && *currentSong == curr) {
        if (curr->next != NULL) {
            *currentSong = curr->next;
        } else if (curr->prev != NULL) {
            *currentSong = curr->prev;
        } else {
            *currentSong = NULL;
        }
    }

    /* Case 1: Only one node in the playlist */
    if (curr == pl->head && curr == pl->tail) {
        pl->head = NULL;
        pl->tail = NULL;
    }
    /* Case 2: Deleting the first (head) node */
    else if (curr == pl->head) {
        pl->head = curr->next;
        pl->head->prev = NULL;
    }
    /* Case 3: Deleting the last (tail) node */
    else if (curr == pl->tail) {
        pl->tail = curr->prev;
        pl->tail->next = NULL;
    }
    /* Case 4: Deleting a middle node */
    else {
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
    }

    /* Free dynamically allocated memory */
    free(curr);
    pl->count--;
    return 1;
}

/**
 * Displays the entire playlist by forward traversal of the Doubly Linked List.
 * Highlights the currently playing song if applicable.
 */
void displayPlaylist(const Playlist* pl, const Song* currentSong) {
    if (pl == NULL || pl->head == NULL) {
        printf("\n>>> Playlist is currently empty. Add some songs first! <<<\n");
        return;
    }

    printf("\n============================ MAIN PLAYLIST (%d Song%s) ============================\n",
           pl->count, pl->count > 1 ? "s" : "");
    printf("%-6s | %-28s | %-22s | %-10s | %s\n",
           "ID", "Title", "Artist", "Duration", "Status");
    printf("-----------------------------------------------------------------------------------\n");

    const Song* temp = pl->head;
    while (temp != NULL) {
        char status[20] = "";
        if (currentSong != NULL && temp->id == currentSong->id) {
            strcpy(status, "[PLAYING]");
        }

        printf("%-6d | %-28s | %-22s | %5.2f mins | %s\n",
               temp->id, temp->title, temp->artist, temp->duration, status);
        temp = temp->next;
    }
    printf("===================================================================================\n");
}

/**
 * Displays detailed information about a single song in a stylized card.
 */
void displaySongCard(const Song* song) {
    if (song == NULL) {
        printf("\n[ No song details to display ]\n");
        return;
    }
    printf("\n========== NOW PLAYING ==========\n");
    printf("  Song ID   : %d\n", song->id);
    printf("  Song Name : %s\n", song->title);
    printf("  Singer    : %s\n", song->artist);
    printf("  Duration  : %.2f mins\n", song->duration);
    printf("=================================\n");
}

/**
 * Frees all memory allocated for songs in the Doubly Linked List.
 */
void freePlaylist(Playlist* pl) {
    if (pl == NULL) return;
    Song* curr = pl->head;
    while (curr != NULL) {
        Song* next = curr->next;
        free(curr);
        curr = next;
    }
    pl->head = NULL;
    pl->tail = NULL;
    pl->count = 0;
}

