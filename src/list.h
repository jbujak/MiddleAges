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
 * Implementation of Iterator design patter.
 * Does not guarantee any particular order.
 * Must be called before first get_next_pawn() after each change.
 * @return First pawn in the list or NULL if list is empty.
 */
struct pawn* get_first_pawn(struct list *list);

/**
 * Implementation of Iterator design patter.
 * Does not guarantee any particular order.
 * After each change get_first_pawn() must be called first.
 * @return Successor of last pawn returned by get_first_pawn() or get_next_pawn() or NULL if successor does not exist.
 */
struct pawn* get_next_pawn(struct list *list);

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

/**
 * Frees list but does not free its element.
 */
void free_list_wihout_content(struct list *list);

#endif //LIST_H
