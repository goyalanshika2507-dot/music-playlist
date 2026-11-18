#include "common.h"

void initHashTable(HashTable* ht)
{
    if (ht==NULL)
    {
        return;
    }
    for (int i=0;i<TABLE_SIZE;i++)
    {
        ht->table[i]=NULL;
    }
}

int hashFunction(int songId)
{
    int index=songId%TABLE_SIZE;
    if(index<0)
    {
        index=index+TABLE_SIZE;
    }
    return index;
}

void insertIntoHashTable(HashTable*ht, Song*song)
{
    if(ht==NULL||song==NULL)
    {
        return;
    }
    if(searchHashTable(ht,song->id)!=NULL)
    {
        printf("Song ID already exists!\n");
        return;
    }
    int index=hashFunction(song->id);
    HashNode*newNode=(HashNode*)malloc(sizeof(HashNode));
    if (newNode==NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }
    newNode->song=song;
    newNode->next=ht->table[index];
    ht->table[index]=newNode;
}

Song*searchHashTable(const HashTable* ht,int songId)
{
    if(ht==NULL)
    {
        return NULL;
    }
    int index=hashFunction(songId);
    HashNode*curr=ht->table[index];
    while (curr!=NULL)
    {
        if(curr->song!=NULL&&curr->song->id==songId)
        {
            return curr->song;
        }
        curr=curr->next;
    }
    return NULL;
}

int deleteFromHashTable(HashTable* ht, int songId)
{
    if(ht==NULL)
    {
        return 0;
    }
    int index=hashFunction(songId);
    HashNode*curr=ht->table[index];
    HashNode* prev = NULL;
    while(curr!=NULL)
    {
        if(curr->song!=NULL&&curr->song->id==songId)
        {
            if(prev==NULL)
            {
                ht->table[index]=curr->next;
            }
            else
            {
                prev->next=curr->next;
            }
            free(curr);
            return 1;
        }
        prev=curr;
        curr=curr->next;
    }
    return 0;
}

void displayHashTable(const HashTable* ht)
{
    if(ht==NULL)
    {
        return;
    }
    printf("\n--- HASH TABLE ---\n");
    for(int i=0;i<TABLE_SIZE;i++)
    {
        printf("Bucket [%d]:",i);
        HashNode*curr=ht->table[i];
        if (curr==NULL)
        {
            printf("Empty\n");
        }
        else
        {
            while(curr!=NULL)
            {
                if (curr->song!=NULL)
                {
                    printf("[ID: %d | Title: %s]-> ",
                           curr->song->id,
                           curr->song->title);
                }
                curr = curr->next;
            }
            printf("NULL\n");
        }
    }
}

void freeHashTable(HashTable* ht)
{
    if(ht==NULL)
    {
        return;
    }
    for(int i=0;i<TABLE_SIZE;i++)
    {
        HashNode*curr=ht->table[i];
        while (curr!=NULL)
        {
            HashNode*next=curr->next;
            free(curr);
            curr=next;
        }
        ht->table[i]=NULL;
    }
}