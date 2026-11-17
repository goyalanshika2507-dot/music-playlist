/**
 * ============================================================================
 * Main Controller: Music Playlist Management System
 * File: main.c
 * 
 * Description:
 * Menu-driven console interface that integrates all four member modules:
 *   - Member 1: Doubly Linked List (Playlist)
 *   - Member 2: Queue (Play Queue - FIFO)
 *   - Member 3: Stack (Recently Played History - LIFO)
 *   - Member 4: Hash Table (Fast Search by ID - Separate Chaining)
 * ============================================================================
 */

#include "common.h"

/* ========================================================================= */
/*                          INPUT HELPER UTILITIES                           */
/* ========================================================================= */

/**
 * Clears remaining characters from the standard input buffer.
 */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* Discard characters until newline or EOF */
    }
}

/**
 * Safely reads a string from the user, removing the trailing newline.
 */
void readString(const char* prompt, char* buffer, int maxLen) {
    printf("%s", prompt);
    if (fgets(buffer, maxLen, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }
}

/**
 * Safely reads an integer from the user with input validation.
 */
int readInteger(const char* prompt) {
    int value;
    int itemsRead;
    while (1) {
        printf("%s", prompt);
        itemsRead = scanf("%d", &value);
        if (itemsRead == 1) {
            clearInputBuffer();
            return value;
        } else {
            printf("Invalid input! Please enter a valid number.\n");
            clearInputBuffer();
        }
    }
}

/**
 * Safely reads a floating-point number from the user with input validation.
 */
float readFloat(const char* prompt) {
    float value;
    int itemsRead;
    while (1) {
        printf("%s", prompt);
        itemsRead = scanf("%f", &value);
        if (itemsRead == 1 && value > 0.0f) {
            clearInputBuffer();
            return value;
        } else {
            printf("Invalid input! Please enter a positive number (e.g. 3.45).\n");
            clearInputBuffer();
        }
    }
}

/* ========================================================================= */
/*                             MENU DISPLAY                                  */
/* ========================================================================= */

void printMenu(void) {
    printf("\n");
    printf("========== MUSIC PLAYLIST MANAGER ==========\n");
    printf(" 1. Add Song\n");
    printf(" 2. Remove Song\n");
    printf(" 3. Display Playlist\n");
    printf(" 4. Search Song by ID\n");
    printf(" 5. Add Song to Queue\n");
    printf(" 6. Display Song Queue\n");
    printf(" 7. Play Next Queued Song\n");
    printf(" 8. Select/Play Song from Playlist\n");
    printf(" 9. Next Song\n");
    printf("10. Previous Song\n");
    printf("11. Mark Current Song as Completed\n");
    printf("12. Display Recently Played History\n");
    printf("13. Remove Latest History Entry\n");
    printf("14. Exit\n");
    printf("============================================\n");
}

/* ========================================================================= */
/*                              MAIN FUNCTION                                */
/* ========================================================================= */

int main(void) {
    /* Initialize all core data structures */
    Playlist playlist;
    Queue playQueue;
    Stack historyStack;
    HashTable hashTable;
    Song* currentSong = NULL;

    initPlaylist(&playlist);
    initQueue(&playQueue);
    initStack(&historyStack);
    initHashTable(&hashTable);

    printf("========================================================\n");
    printf("  Welcome to the Music Playlist Management System (C)   \n");
    printf("  Data Structures: DLL + Queue + Stack + Hash Table     \n");
    printf("========================================================\n");

    int choice = 0;

    while (choice != 14) {
        printMenu();
        choice = readInteger("Enter your choice (1-14): ");

        switch (choice) {
            case 1: {
                /* --- Option 1: Add Song --- */
                printf("\n--- ADD NEW SONG ---\n");
                int id = readInteger("Enter unique Song ID (positive integer): ");
                if (id <= 0) {
                    printf("Error: Song ID must be a positive number!\n");
                    break;
                }

                /* Check if ID already exists in Hash Table */
                if (searchHashTable(&hashTable, id) != NULL) {
                    printf("Error: A song with ID %d already exists in the playlist!\n", id);
                    break;
                }

                char title[STRING_MAX];
                char artist[STRING_MAX];
                readString("Enter Song Title: ", title, STRING_MAX);
                if (strlen(title) == 0) {
                    strcpy(title, "Untitled Track");
                }

                readString("Enter Singer / Artist Name: ", artist, STRING_MAX);
                if (strlen(artist) == 0) {
                    strcpy(artist, "Unknown Artist");
                }

                float duration = readFloat("Enter Duration in minutes (e.g. 3.45): ");

                /* Create dynamically allocated song node */
                Song* newSong = createSong(id, title, artist, duration);
                if (newSong == NULL) {
                    printf("Error: Could not allocate memory for song!\n");
                    break;
                }

                /* Insert into Doubly Linked List */
                addSongToPlaylist(&playlist, newSong);

                /* Insert reference into Hash Table */
                insertIntoHashTable(&hashTable, newSong);

                /* If no song is currently playing, default to this first song */
                if (currentSong == NULL) {
                    currentSong = newSong;
                }

                printf("Success: Song '%s' (ID: %d) added to playlist and indexed in Hash Table.\n",
                       newSong->title, newSong->id);
                break;
            }

            case 2: {
                /* --- Option 2: Remove Song --- */
                printf("\n--- REMOVE SONG ---\n");
                if (playlist.head == NULL) {
                    printf("Playlist is already empty. Nothing to remove.\n");
                    break;
                }

                int id = readInteger("Enter Song ID to remove: ");
                Song* target = searchHashTable(&hashTable, id);

                if (target == NULL) {
                    printf("Error: Song ID %d not found in playlist.\n", id);
                    break;
                }

                char deletedTitle[STRING_MAX];
                strncpy(deletedTitle, target->title, STRING_MAX - 1);
                deletedTitle[STRING_MAX - 1] = '\0';

                /* Step A: If target is currently playing, inform user */
                if (currentSong != NULL && currentSong->id == id) {
                    printf("Notice: Currently playing song '%s' is being deleted.\n", deletedTitle);
                }

                /* Step B: Remove any pending occurrences from Queue */
                removeSongFromQueue(&playQueue, id);

                /* Step C: Remove any occurrences from History Stack */
                removeSongFromStack(&historyStack, id);

                /* Step D: Remove reference from Hash Table */
                deleteFromHashTable(&hashTable, id);

                /* Step E: Unlink and free from Doubly Linked List */
                removeSongFromPlaylist(&playlist, id, &currentSong);

                printf("Success: Song '%s' (ID: %d) deleted from playlist and cleaned from all queues/history.\n",
                       deletedTitle, id);
                break;
            }

            case 3: {
                /* --- Option 3: Display Playlist --- */
                displayPlaylist(&playlist, currentSong);
                break;
            }

            case 4: {
                /* --- Option 4: Search Song by ID (Hashing) --- */
                printf("\n--- SEARCH SONG BY ID (HASHING) ---\n");
                int id = readInteger("Enter Song ID to search: ");
                Song* found = searchHashTable(&hashTable, id);

                if (found != NULL) {
                    printf("\n[Match Found via Hash Table in O(1) average time!]\n");
                    displaySongCard(found);
                } else {
                    printf("Result: Song with ID %d was NOT found in the playlist.\n", id);
                }
                break;
            }

            case 5: {
                /* --- Option 5: Add Song to Queue --- */
                printf("\n--- ADD SONG TO PLAY QUEUE ---\n");
                if (playlist.head == NULL) {
                    printf("Playlist is empty. Add songs to the playlist first!\n");
                    break;
                }

                int id = readInteger("Enter Song ID to add to queue: ");
                Song* song = searchHashTable(&hashTable, id);

                if (song == NULL) {
                    printf("Error: Song ID %d does not exist in the playlist.\n", id);
                } else {
                    enqueueSong(&playQueue, song);
                }
                break;
            }

            case 6: {
                /* --- Option 6: Display Song Queue --- */
                displayQueue(&playQueue);
                break;
            }

            case 7: {
                /* --- Option 7: Play Next Queued Song --- */
                printf("\n--- PLAY NEXT QUEUED SONG ---\n");
                if (isQueueEmpty(&playQueue)) {
                    printf("Play Queue is empty. No songs waiting in queue!\n");
                } else {
                    Song* nextUp = dequeueSong(&playQueue);
                    currentSong = nextUp;
                    printf(">> Dequeued next song for playback.\n");
                    displaySongCard(currentSong);
                }
                break;
            }

            case 8: {
                /* --- Option 8: Select/Play Song from Playlist --- */
                printf("\n--- SELECT SONG FROM PLAYLIST ---\n");
                if (playlist.head == NULL) {
                    printf("Playlist is empty. Add songs first!\n");
                    break;
                }

                int id = readInteger("Enter Song ID to select and play: ");
                Song* song = searchHashTable(&hashTable, id);

                if (song == NULL) {
                    printf("Error: Song ID %d not found in playlist.\n", id);
                } else {
                    currentSong = song;
                    displaySongCard(currentSong);
                }
                break;
            }

            case 9: {
                /* --- Option 9: Next Song --- */
                printf("\n--- NAVIGATE: NEXT SONG ---\n");
                if (playlist.head == NULL) {
                    printf("Playlist is empty.\n");
                } else if (currentSong == NULL) {
                    currentSong = playlist.head;
                    displaySongCard(currentSong);
                } else if (currentSong->next != NULL) {
                    currentSong = currentSong->next;
                    displaySongCard(currentSong);
                } else {
                    printf("End of playlist reached! There is no next song.\n");
                    printf("Currently at the last song:\n");
                    displaySongCard(currentSong);
                }
                break;
            }

            case 10: {
                /* --- Option 10: Previous Song --- */
                printf("\n--- NAVIGATE: PREVIOUS SONG ---\n");
                if (playlist.head == NULL) {
                    printf("Playlist is empty.\n");
                } else if (currentSong == NULL) {
                    currentSong = playlist.tail;
                    displaySongCard(currentSong);
                } else if (currentSong->prev != NULL) {
                    currentSong = currentSong->prev;
                    displaySongCard(currentSong);
                } else {
                    printf("Beginning of playlist reached! There is no previous song.\n");
                    printf("Currently at the first song:\n");
                    displaySongCard(currentSong);
                }
                break;
            }

            case 11: {
                /* --- Option 11: Mark Current Song as Completed --- */
                printf("\n--- MARK PLAYBACK COMPLETED ---\n");
                if (currentSong == NULL) {
                    printf("No song is currently selected or playing.\n");
                } else {
                    printf(">> Playback simulated successfully for: '%s' by %s (%.2f mins)\n",
                           currentSong->title, currentSong->artist, currentSong->duration);
                    pushHistory(&historyStack, currentSong);
                }
                break;
            }

            case 12: {
                /* --- Option 12: Display Recently Played History --- */
                displayHistory(&historyStack);
                break;
            }

            case 13: {
                /* --- Option 13: Remove Latest History Entry --- */
                printf("\n--- REMOVE LATEST HISTORY ENTRY ---\n");
                if (isStackEmpty(&historyStack)) {
                    printf("History is already empty. Nothing to remove.\n");
                } else {
                    Song* popped = popHistory(&historyStack);
                    if (popped != NULL) {
                        printf(">> Successfully removed '%s' (ID: %d) from top of history stack.\n",
                               popped->title, popped->id);
                    }
                }
                break;
            }

            case 14: {
                /* --- Option 14: Exit --- */
                printf("\nExiting Music Playlist Manager...\n");
                printf("Cleaning up dynamically allocated memory across all modules:\n");
                printf(" - Freeing Queue nodes...\n");
                freeQueue(&playQueue);
                printf(" - Freeing Stack nodes...\n");
                freeStack(&historyStack);
                printf(" - Freeing Hash Table collision chains...\n");
                freeHashTable(&hashTable);
                printf(" - Freeing Doubly Linked List songs...\n");
                freePlaylist(&playlist);
                printf("Memory cleanup complete.\n");
                printf("Thank you for using Music Playlist Manager. Goodbye!\n");
                break;
            }

            default:
                printf("\nInvalid choice (%d)! Please choose a number between 1 and 14.\n", choice);
                break;
        }
    }

    return 0;
}

