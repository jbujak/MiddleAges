/** @file
 *  Implemention of list.
 */
#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "pawn.h"

struct list {
	struct list_element *head;
	struct list_element *tail;
};

struct list_element {
	struct pawn *value;
	struct list_element *next;
	struct list_element *prev;
};


static struct list_element* find_element(int x, int y, const struct list* list);

struct list* create_list() {
	struct list *new_list;
	struct list_element *head;
	struct list_element *tail;

	new_list = malloc(sizeof(struct list));

	head = malloc(sizeof(struct list_element));
	tail = malloc(sizeof(struct list_element));

	head->value = NULL;
	head->next = NULL;
	head->prev = tail;
	new_list->head = head;

	tail->value = NULL;
	tail->next = head;
	tail->prev = NULL;
	new_list->tail = tail;

	return new_list;
}

struct pawn* get_pawn(int x, int y, const struct list *list) {
	struct list_element *found;

	found = find_element(x, y, list);

	if(found == NULL)
		return NULL;

	return found->value;
}

void add_pawn(struct pawn *new_pawn, struct list *list) {
	struct list_element *new_element;

	new_element = malloc(sizeof(struct list_element));
	new_element->value = new_pawn;
	new_element->next = list->head;
	new_element->prev = list->head->prev;

	list->head->prev->next = new_element;
	list->head->prev = new_element;
}

struct pawn* remove_pawn(int x, int y, struct list *list) {
	struct list_element *removed;
	struct list_element *next;
	struct list_element *prev;
	struct pawn *result;

	removed = find_element(x, y, list);
	if(removed == NULL)
		return NULL;

	next = removed->next;
	prev = removed->prev;

	next->prev = prev;
	prev->next = next;

	result = removed->value;
	free(removed);

	return result;
}

void free_list(struct list *list) {
	struct list_element *current;
	struct list_element *next;

	current = list->tail->next;

	while(current->next != NULL) {
		next = current->next;

		free(current->value);
		free(current);

		current = next;
	}

	free(list->head);
	free(list->tail);
	free(list);
}

static struct list_element* find_element(int x, int y, const struct list* list) {
	struct list_element *current;

	current = list->tail->next;

	while(current->next != NULL) {
		if(current->value->x == x && current->value->y == y)
			return current;

		current = current->next;
	}

	return NULL;
}
