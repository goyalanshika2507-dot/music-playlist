#include "common.h"

void initPlaylist(Playlist *p){
    if(p==NULL){
        return;
    }
    p->head=NULL;
    p->tail=NULL;
    p->count=0;
}

Song* createSong(int id, const char* title, const char* artist, float duration){
    Song *newsong=(Song*)malloc(sizeof(Song));
    if(newsong==NULL){
        printf("Memory allocation failed!\n");
        return NULL;
    }
    newsong->id=id;
    strncpy(newsong->title, title, STRING_MAX - 1);
    newsong->title[STRING_MAX - 1] = '\0';
    
    strncpy(newsong->artist, artist, STRING_MAX - 1);
    newsong->artist[STRING_MAX - 1] = '\0';

    newsong->duration=duration;
    newsong->prev=NULL;
    newsong->next=NULL;
    
    return newsong;
}

int addSongToPlaylist(Playlist* p, Song* newsong){
    if(p==NULL || newsong==NULL){
        return 0;
    }

    if(p->head==NULL){
        p->head=newsong;
        p->tail=newsong;
        newsong->prev=NULL;
        newsong->next=NULL;
    }
    else{
        p->tail->next=newsong;
        newsong->prev=p->tail;
        newsong->next=NULL;
        p->tail=newsong;
    }

    p->count++;
    return 1;
}

int removeSongFromPlaylist(Playlist *p, int id, Song** currentsong){
    if(p==NULL || p->head==NULL){
        return 0;
    }

    Song *curr=p->head;

    while(curr!=NULL && curr->id!=id){
        curr=curr->next;
    }

    if(curr==NULL){
        printf("Song Not found!");
        return 0;
    }

    if(currentsong!=NULL && *currentsong==curr){
        if(curr->next!=NULL){
            *currentsong=curr->next;
        }
        else if(curr->prev!=NULL){
            *currentsong=curr->prev;
        }
        else{
            *currentsong=NULL;
        }
    }

    if(curr==p->head && curr==p->tail){
        p->head=NULL;
        p->tail=NULL;
    }
    else if(curr==p->head){
        p->head=curr->next;
        p->head->prev=NULL;
    }
    else if(curr==p->tail){
        p->tail=curr->prev;
        p->tail->next=NULL;
    }
    else{
        curr->prev->next=curr->next;
        curr->next->prev=curr->prev;
    }

    free(curr);
    p->count--;
    return 1;
}

void displayPlaylist(const Playlist *p, const Song* currentsong){
    if(p==NULL || p->head ==NULL){
        printf("\nPlaylist is currently empty. Add some songs first!\n");
        return;
    }

    printf("\n============================ MAIN PLAYLIST ============================\n");
    printf("Total Songs: %d\n", p->count);

    printf("ID\tTitle\t\tArtist\t\tDuration\tStatus\n");

    const Song *temp=p->head;
    while(temp!=NULL){
        char status[20]="";
        if(currentsong!=NULL && temp->id==currentsong->id){
            strcpy(status, "[PLAYING]");
        }
        printf("%d | %s | %s | %.2f mins | %s\n", temp->id, temp->title, temp->artist, temp->duration, status);
        temp=temp->next;
    }
}

void displaySongCard(const Song *song){
    if (song == NULL) {
        printf("\n[ No song details to display ]\n");
        return;
    }
    printf("\n========== NOW PLAYING ==========\n");
    printf("Song ID   : %d\n", song->id);
    printf("Song Name : %s\n", song->title);
    printf("Singer    : %s\n", song->artist);
    printf("Duration  : %.2f mins\n", song->duration);
}

void freePlaylist(Playlist *p){
    if(p==NULL)
        return;
    Song* curr=p->head;
    while (curr!=NULL) {
        Song* next=curr->next;
        free(curr);
        curr=next;
    }
    p->head=NULL;
    p->tail=NULL;
    p->count=0;
}