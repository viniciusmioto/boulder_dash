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

void generate_name(char *string);

void init_list(LinkedList *list);

bool empty_list(LinkedList list);

void print_list(LinkedList list);

void deallocate_list(LinkedList *list);

int insert_beggining(LinkedList *list, int value, char name[25]);

/* Insere o elemento score no final da fila; Retorna 1 se a operação foi bem sucedida e zero caso contrário; */
int insert_end(LinkedList *list, int value, char name[25]);

int save_score(LinkedList *list, int value, char name[25]);

void read_scores(LinkedList *list);

int remove_first(LinkedList *list);

int remove_last_score(LinkedList *list, SCORES *score);

void write_scores(LinkedList list);