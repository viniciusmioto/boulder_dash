#include "scores.h"

/* Cria uma fila vazia, isto é, head e tail apontam para NULL; */
void init_list(LinkedList *list)
{
	list->head = NULL;
	list->tail = NULL;
}

/* Retorna 1 se a fila está vazia e zero caso contrário; */
bool empty_list(LinkedList list)
{
	return list.head == NULL;
}

/* Remove todos os elementos da fila e faz com que head e tail apontem para NULL; */
void deallocate_list(LinkedList *list)
{
	SCORES *curr = list->head;

	if (list->head == NULL)
		return;

	while (curr->next != NULL)
	{
		curr = curr->next;
		free(curr->prev);
	}

	free(curr);
	list->tail = NULL;
	list->head = NULL;
}

/* Insere o elemento score no início da fila; Retorna 1 se a operação foi bem sucedida e zero caso contrário; */
int insert_beggining(LinkedList *list, int value, char name[5])
{
	SCORES *new_score = malloc(sizeof(SCORES));

	if (new_score == NULL)
		return 0;

	new_score->value = value;
	new_score->prev = NULL;
	strcpy(new_score->name, name);

	if (!empty_list(*list))
	{
		new_score->next = list->head;
		(list->head)->prev = new_score;
		list->head = new_score;
	}
	else
	{
		new_score->next = NULL;
		list->head = new_score;
		list->tail = new_score;
	}

	return 1;
}

/* Insere o elemento score no final da fila; Retorna 1 se a operação foi bem sucedida e zero caso contrário; */
int insert_end(LinkedList *list, int value, char name[5])
{
	SCORES *new_score = malloc(sizeof(SCORES));

	if (new_score == NULL)
		return 0;

	if (empty_list(*list))
	{
		free(new_score);
		return insert_beggining(list, value, name);
	}

	new_score->value = value;
	strcpy(new_score->name, name);
	new_score->next = NULL;
	new_score->prev = list->tail;
	(list->tail)->next = new_score;
	list->tail = new_score;
	return 1;
}

/* Recebe uma fila, uma prioridade (score) e uma senha 
 * insere o elemento na fila, respeitando as prioridades */
int save_score(LinkedList *list, int value, char name[5])
{
	SCORES *curr;
	SCORES *new_score = malloc(sizeof(SCORES));

	/* caso esse seja o primeiro elemento da fila */
	if (empty_list(*list) || value > list->head->value)
	{
		free(new_score);
		return insert_beggining(list, value, name);
	}

	/* caso seja o último da fila  */
	if (value <= list->tail->value)
	{
		free(new_score);
		return insert_end(list, value, name);
	}

	if (new_score == NULL)
		return 0;

	for (curr = list->head; curr->value >= value; curr = curr->next)
		;

	new_score->value = value;
	strcpy(new_score->name, name);
	new_score->prev = curr->prev;
	new_score->next = curr;
	curr->prev = new_score;
	(new_score->prev)->next = new_score;

	return 1;
}

/* Read name and score from file and insert in order */
void read_scores(LinkedList *list)
{
	FILE *file;
	int score;
	char name[25];

	file = fopen("./resources/scores.txt", "r");

	if (file == NULL)
		return;

	while (fscanf(file, "%s %d", name, &score) != EOF)
		save_score(list, score, name);

	fclose(file);
}

/* Remove o primeiro elemento da fila. Retorna 1 se a operação foi bem sucedida e zero caso contrário; */
int remove_first(LinkedList *list)
{
	if (empty_list(*list))
		return 0;

	SCORES *curr = list->head;

	if (list->head->next == NULL)
	{
		deallocate_list(list);
		return 1;
	}

	list->head = list->head->next;
	list->head->prev = NULL;
	curr->prev = NULL;
	curr->next = NULL;
	free(curr);
	return 1;
}

/* Remove o último elemento da fila. Retorna 1 se a operação foi bem sucedida e zero caso contrário; */
int remove_last_score(LinkedList *list, SCORES *score)
{
	if (empty_list(*list))
		return 0;

	if (score->prev != NULL)
		score->prev->next = score->next;

	if (score->next != NULL)
		score->next->prev = score->prev;

	list->tail = score->prev;
	free(score);
	return 1;
}

/* Overwrite scores file with the 10 best scores */
void write_scores(LinkedList list)
{
	FILE *file;
	SCORES *curr;
	int i = 0;

	file = fopen("./resources/scores.txt", "w");

	if (file == NULL)
		return;

	for (curr = list.head; curr != NULL && i < 10; curr = curr->next)
	{
		fprintf(file, "%s %d\n", curr->name, curr->value);
		i++;
	}

	fclose(file);
}
