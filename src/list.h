/** @file
 *  Interface of list.
 */

#ifndef LIST_H
#define LIST_H

#include "pawn.h"

/**
 * Structure representing a list.
 * Should be accesed only using functions from this file.
 */
struct list;

/**
 * Creates new list.
 * Allocates memory for new list and initializes it.
 * @return New empty list.
 */
struct list* create_list();

/**
 * Finds pawn with given coordinates in list.
 * @return Pawn at position (x, y) or NULL if such a pawn does not exist.
 */
struct pawn* get_pawn(int x, int y, const struct list *list);

/**
 * Adds pawn to list.
 */
void add_pawn(struct pawn *new_pawn, struct list *list);

/**
 * Removes pawn at given coordinates from list.
 * @return Removed pawn or NULL if pawn at (x, y) does not exist.
 */
struct pawn* remove_pawn(int x, int y, struct list *list);

/**
 * Completely frees list and all elements in it.
 */
void free_list(struct list *list);

#endif //LIST_H
