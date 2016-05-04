/** @file
 *  Interface of preview engine.
 */
#ifndef PREVIEW_H
#define PREVIEW_H

#include "pawn.h"

/**
 * Sets preview size and initializes it.
 * @param[in] n Size of board.
 */
void init_preview(int n);

/**
 * Puts pawn at given position.
 * @param[in] player Player to whom belongs pawn.
 * @param[in] x Column for new pawn.
 * @param[in] y Row for new pawn.
 * @param[in] pawn_type Type of pawn to be placed.
 */
void update_preview_add(enum player player, int x, int y, enum pawn_type pawn_type);

/**
 * Removes pawn from given position.
 * @param[in] x Column with removed pawn.
 * @param[in] y Row with removed pawn.
 */
void update_preview_remove(int x, int y);

/**
 * Prints board preview to stdin.
 */
void print_preview();

#endif //PREVIEW_H
