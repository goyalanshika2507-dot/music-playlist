# Music Playlist Management System (Data Structures in C)

A modular, terminal-based **Music Playlist Management System** developed in C using fundamental Data Structures: **Doubly Linked List (DLL)**, **Queue**, **Stack**, and **Hash Table (Separate Chaining)**. 

Designed specifically for College Data Structures Problem-Based Learning (PBL) and lab projects.

---

## 1. Project Overview

This project simulates a complete music player management console where users can manage songs, organize an upcoming playback queue, track playback history, and perform instantaneous song searches using hashing. 

Audio playback is simulated via styled terminal status displays.

### Key Highlights
- **Doubly Linked List (DLL)** for the primary playlist with bidirectional navigation (`Next` / `Previous`).
- **Queue (FIFO)** for scheduling upcoming songs.
- **Stack (LIFO)** for logging recently played song history.
- **Hash Table with Separate Chaining** for $O(1)$ average-time song lookups by ID.
- **Safe Memory Management**: Dynamic memory allocation via `malloc()` and thorough deallocation via `free()` to prevent memory leaks and dangling pointers.

---

## 2. Folder Structure

```text
Music_Playlist_Manager/
│
├── common.h             # Common structures, prototypes, and constants
├── member1_dll.c        # Doubly Linked List implementation (Main Playlist)
├── member2_queue.c      # Queue implementation (Play Queue - FIFO)
├── member3_stack.c      # Stack implementation (Recently Played History - LIFO)
├── member4_hashing.c    # Hash Table implementation (Search & Indexing)
├── main.c               # Menu controller, user I/O, and module integration
└── README.md            # Project documentation and compilation guide
```

---

## 3. Team Member Contributions & File Responsibilities

| Member | File | Assigned Data Structure | Core Responsibilities |
|---|---|---|---|
| **Member 1** | `member1_dll.c` | **Doubly Linked List (DLL)** | Main playlist management: adding songs, removing songs by ID (first, middle, last nodes), bidirectional navigation (`Next`/`Prev`), maintaining current song pointer, displaying playlist. |
| **Member 2** | `member2_queue.c` | **Queue (FIFO)** | Upcoming song queue using a linked list: enqueue, dequeue for playback, display queue, empty queue validation, and synchronized cleanup when songs are deleted. |
| **Member 3** | `member3_stack.c` | **Stack (LIFO)** | Recently played history using a linked list: pushing completed songs, popping recent entries, displaying history top-to-bottom without popping, and cleanup. |
| **Member 4** | `member4_hashing.c` | **Hash Table** | Fast song searching using a modulo hash function (`songID % TABLE_SIZE`), collision resolution using Separate Chaining (singly linked lists), inserting/deleting node references, and table visualization. |
| **Team Integration** | `main.c`, `common.h` | **Integration & UI** | Shared `Song` structure, menu handling (14 options), input validation (safe string & integer reading), and memory safety synchronization. |

---

## 4. How the Four Modules Communicate

To maintain memory efficiency and prevent duplicate records, the system utilizes a **Master-Reference Architecture**:

1. **Master Records (DLL)**:
   - The master copy of each song node exists exclusively within Member 1's Doubly Linked List (`Playlist`).
2. **References in Hash Table**:
   - When a song is added, Member 4 inserts a pointer (`Song*`) into the Hash Table. Searching by ID executes in $O(1)$ average time and directly returns the DLL node.
3. **References in Queue**:
   - When a user enqueues a song, Member 2 adds a `QueueNode` storing a pointer (`Song*`) to the existing DLL song.
4. **References in History Stack**:
   - When a song finishes playback, Member 3 pushes a `StackNode` referencing that `Song*` onto the history stack.
5. **Synchronized Deletion (Preventing Dangling Pointers)**:
   - When a song is removed from the playlist, `main.c` orchestrates safe unlinking across all modules in order:
     1. Unlink reference from the **Queue** (`removeSongFromQueue`).
     2. Unlink reference from the **Stack** (`removeSongFromStack`).
     3. Remove from the **Hash Table** (`deleteFromHashTable`).
     4. Safely free and unlink the node from the **Doubly Linked List** (`removeSongFromPlaylist`).

---

## 5. Main Menu Options

```text
========== MUSIC PLAYLIST MANAGER ==========
 1. Add Song
 2. Remove Song
 3. Display Playlist
 4. Search Song by ID
 5. Add Song to Queue
 6. Display Song Queue
 7. Play Next Queued Song
 8. Select/Play Song from Playlist
 9. Next Song
10. Previous Song
11. Mark Current Song as Completed
12. Display Recently Played History
13. Remove Latest History Entry
14. Exit
============================================
```

---

## 6. Compilation & Execution Guide

### Prerequisites
- GCC Compiler (MinGW for Windows, or standard GCC for Linux/macOS)
- Terminal / Command Prompt / PowerShell

### Compilation Command

Open your terminal, navigate to the `Music_Playlist_Manager` folder, and compile all files together:

```bash
gcc -Wall -Wextra -std=c99 -o playlist_manager main.c member1_dll.c member2_queue.c member3_stack.c member4_hashing.c
```

### Execution Command

- **On Windows (Command Prompt or PowerShell):**
  ```powershell
  .\playlist_manager.exe
  ```

- **On Linux / macOS:**
  ```bash
  ./playlist_manager
  ```

---

## 7. Test Cases & Verification Checklist

| Scenario | Input / Action | Expected Behavior | Status |
|---|---|---|---|
| **1. Empty Playlist Display** | Select option `3` on fresh launch | Displays "Playlist is currently empty." | Passed |
| **2. Unsuccessful Search** | Select option `4`, enter ID `99` | Displays "Song with ID 99 was NOT found." | Passed |
| **3. Empty Removal** | Select option `2` on empty playlist | Gracefully reports playlist is empty. | Passed |
| **4. Adding Songs** | Select option `1`, enter ID, Name, Artist, Duration | Successfully creates DLL node & indexes in Hash Table. | Passed |
| **5. Duplicate ID Rejection** | Enter existing ID in option `1` | Displays duplicate ID error. | Passed |
| **6. Queue FIFO Order** | Enqueue Song 101, then Song 111 | Song 101 is dequeued first, followed by Song 111. | Passed |
| **7. Stack LIFO History** | Mark Song A complete, then Song B | History displays Song B at Top (#1), Song A at (#2). | Passed |
| **8. Hash Collisions** | Add IDs `101`, `111`, `121` (`% 10 == 1`) | Successfully chains into Bucket 1 and retrieves each correctly. | Passed |
| **9. Deletion of First/Mid/Last** | Delete Head, Middle, and Tail songs | DLL pointers (`prev`/`next`) adjust without breaking chain. | Passed |
| **10. Clean Exit** | Select option `14` | Frees Queue, Stack, Hash Table, and Playlist nodes cleanly. | Passed |

---

## 8. Code Simplicity & Academic Standards
- Pure C language implementation (C99 standard).
- Standard headers only (`<stdio.h>`, `<stdlib.h>`, `<string.h>`).
- Safe inputs handling spaces via `fgets()`.
- Thoroughly documented code comments for viva questions and project presentation.

## 9. commands to run
1. cd .\Music_Playlist_Manager
2. gcc main.c member1_dll.c member2_queue.c member3_stack.c member4_hashing.c -o playlist_manager.exe
3. .\playlist_manager.exe