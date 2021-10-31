#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct SCORES
{
    char name[20];
    int value;
    struct SCORES *next, *prev;
} SCORES;

typedef struct LinkedList
{
    SCORES *tail, *head;  
} LinkedList;

void init_list(LinkedList *list);

bool empty_list(LinkedList list);

void deallocate_list(LinkedList *list);

void insert_beggining(LinkedList *list, int value, char name[25]);

void insert_end(LinkedList *list, int value, char name[25]);

void save_score(LinkedList *list, int value, char name[25]);

void read_scores(LinkedList *list);

void write_scores(LinkedList list);