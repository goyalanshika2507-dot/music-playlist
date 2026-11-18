#include "common.h"

void clearInputBuffer(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {
        // Clear input buffer
    }
}

void readString(const char* prompt, char* buffer, int maxLen)
{
    printf("%s", prompt);

    if (fgets(buffer, maxLen, stdin) != NULL)
    {
        size_t len = strlen(buffer);

        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
        }
    }
    else
    {
        buffer[0] = '\0';
    }
}

int readInteger(const char* prompt)
{
    int value;
    int itemsRead;

    while (1)
    {
        printf("%s", prompt);

        itemsRead = scanf("%d", &value);

        if (itemsRead == 1)
        {
            clearInputBuffer();
            return value;
        }
        else
        {
            printf("Invalid input! Enter a valid number.\n");
            clearInputBuffer();
        }
    }
}

float readFloat(const char* prompt)
{
    float value;
    int itemsRead;

    while (1)
    {
        printf("%s", prompt);

        itemsRead = scanf("%f", &value);

        if (itemsRead == 1 && value > 0.0f)
        {
            clearInputBuffer();
            return value;
        }
        else
        {
            printf("Invalid input! Enter a positive number.\n");
            clearInputBuffer();
        }
    }
}


void printMenu(void)
{
    printf("\n========== MUSIC PLAYLIST MANAGER ==========\n");

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

int main(void)
{
    Playlist playlist;
    Queue playQueue;
    Stack historyStack;
    HashTable hashTable;

    Song* currentSong = NULL;

    initPlaylist(&playlist);
    initQueue(&playQueue);
    initStack(&historyStack);
    initHashTable(&hashTable);

    printf("\n============================================\n");
    printf(" MUSIC PLAYLIST MANAGEMENT SYSTEM\n");
    printf(" Data Structures: DLL + Queue + Stack + Hash Table\n");
    printf("============================================\n");

    int choice = 0;

    while (choice != 14)
    {
        printMenu();

        choice = readInteger("Enter your choice (1-14): ");

        switch (choice)
        {
            case 1:
            {
                printf("\n--- ADD NEW SONG ---\n");

                int id = readInteger("Enter Song ID: ");

                if (id <= 0)
                {
                    printf("Song ID must be positive!\n");
                    break;
                }

                if (searchHashTable(&hashTable, id) != NULL)
                {
                    printf("Song ID already exists!\n");
                    break;
                }

                char title[STRING_MAX];
                char artist[STRING_MAX];

                readString("Enter Song Title: ", title, STRING_MAX);

                if (strlen(title) == 0)
                {
                    strcpy(title, "Untitled Track");
                }

                readString("Enter Artist Name: ", artist, STRING_MAX);

                if (strlen(artist) == 0)
                {
                    strcpy(artist, "Unknown Artist");
                }

                float duration = readFloat("Enter Duration: ");

                Song* newSong = createSong(id, title, artist, duration);

                if (newSong == NULL)
                {
                    printf("Memory allocation failed!\n");
                    break;
                }

                addSongToPlaylist(&playlist, newSong);

                insertIntoHashTable(&hashTable, newSong);

                if (currentSong == NULL)
                {
                    currentSong = newSong;
                }

                printf("Song added successfully!\n");

                break;
            }

            case 2:
            {
                printf("\n--- REMOVE SONG ---\n");

                if (playlist.head == NULL)
                {
                    printf("Playlist is empty!\n");
                    break;
                }

                int id = readInteger("Enter Song ID to remove: ");

                Song* target = searchHashTable(&hashTable, id);

                if (target == NULL)
                {
                    printf("Song not found!\n");
                    break;
                }

                char deletedTitle[STRING_MAX];

                strncpy(deletedTitle, target->title, STRING_MAX - 1);
                deletedTitle[STRING_MAX - 1] = '\0';

                removeSongFromQueue(&playQueue, id);

                removeSongFromStack(&historyStack, id);

                deleteFromHashTable(&hashTable, id);

                removeSongFromPlaylist(&playlist, id, &currentSong);

                printf("Song '%s' deleted successfully!\n", deletedTitle);

                break;
            }

            case 3:
            {
                displayPlaylist(&playlist, currentSong);
                break;
            }

            case 4:
            {
                printf("\n--- SEARCH SONG USING HASH TABLE ---\n");

                int id = readInteger("Enter Song ID: ");

                Song* found = searchHashTable(&hashTable, id);

                if (found != NULL)
                {
                    printf("Song found!\n");
                    displaySongCard(found);
                }
                else
                {
                    printf("Song not found!\n");
                }

                break;
            }

            case 5:
            {
                printf("\n--- ADD SONG TO QUEUE ---\n");

                if (playlist.head == NULL)
                {
                    printf("Playlist is empty!\n");
                    break;
                }

                int id = readInteger("Enter Song ID: ");

                Song* song = searchHashTable(&hashTable, id);

                if (song == NULL)
                {
                    printf("Song not found!\n");
                }
                else
                {
                    enqueueSong(&playQueue, song);
                }

                break;
            }

            case 6:
            {
                displayQueue(&playQueue);
                break;
            }

            case 7:
            {
                printf("\n--- PLAY NEXT QUEUED SONG ---\n");

                if (isQueueEmpty(&playQueue))
                {
                    printf("Queue is empty!\n");
                }
                else
                {
                    currentSong = dequeueSong(&playQueue);

                    printf("Playing next queued song:\n");

                    displaySongCard(currentSong);
                }

                break;
            }

            case 8:
            {
                printf("\n--- SELECT SONG ---\n");

                if (playlist.head == NULL)
                {
                    printf("Playlist is empty!\n");
                    break;
                }

                int id = readInteger("Enter Song ID: ");

                Song* song = searchHashTable(&hashTable, id);

                if (song == NULL)
                {
                    printf("Song not found!\n");
                }
                else
                {
                    currentSong = song;

                    displaySongCard(currentSong);
                }

                break;
            }

            case 9:
            {
                printf("\n--- NEXT SONG ---\n");

                if (playlist.head == NULL)
                {
                    printf("Playlist is empty!\n");
                }
                else if (currentSong == NULL)
                {
                    currentSong = playlist.head;
                    displaySongCard(currentSong);
                }
                else if (currentSong->next != NULL)
                {
                    currentSong = currentSong->next;
                    displaySongCard(currentSong);
                }
                else
                {
                    printf("Already at the last song!\n");
                }

                break;
            }

            case 10:
            {
                printf("\n--- PREVIOUS SONG ---\n");

                if (playlist.head == NULL)
                {
                    printf("Playlist is empty!\n");
                }
                else if (currentSong == NULL)
                {
                    currentSong = playlist.tail;
                    displaySongCard(currentSong);
                }
                else if (currentSong->prev != NULL)
                {
                    currentSong = currentSong->prev;
                    displaySongCard(currentSong);
                }
                else
                {
                    printf("Already at the first song!\n");
                }

                break;
            }

            case 11:
            {
                printf("\n--- PLAYBACK COMPLETED ---\n");

                if (currentSong == NULL)
                {
                    printf("No song selected!\n");
                }
                else
                {
                    printf("Completed: %s\n", currentSong->title);

                    pushHistory(&historyStack, currentSong);
                }

                break;
            }

            case 12:
            {
                displayHistory(&historyStack);
                break;
            }

            case 13:
            {
                printf("\n--- REMOVE LATEST HISTORY ---\n");

                if (isStackEmpty(&historyStack))
                {
                    printf("History is empty!\n");
                }
                else
                {
                    Song* popped = popHistory(&historyStack);

                    if (popped != NULL)
                    {
                        printf("Removed: %s\n", popped->title);
                    }
                }

                break;
            }

            case 14:
            {
                printf("\nExiting Music Playlist Manager...\n");

                freeQueue(&playQueue);

                freeStack(&historyStack);

                freeHashTable(&hashTable);

                freePlaylist(&playlist);

                printf("All allocated memory released.\n");

                printf("Thank you for using the system!\n");

                break;
            }


            default:
            {
                printf("Invalid choice! Enter between 1 and 14.\n");
            }

        }
    }

    return 0;
}