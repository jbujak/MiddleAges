#ifndef LIST_H
#define LIST_H

#include "pawn.h"

struct list;

struct list* create_list();

struct pawn* get_pawn(int x, int y, const struct list *list);

void add_pawn(struct pawn *new_pawn, struct list *list);

struct pawn* remove_pawn(int x, int y, struct list *list);

void free_list(struct list *list);

#endif //LIST_H
