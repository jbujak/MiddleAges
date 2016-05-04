/** @file
 *  Interface of pawn management and definition of pawn types.
 */

#ifndef PAWN_H
#define PAWN_H

#include "global.h"

/** 
 * Possible types of pawns sorted by increasing power.
 */
enum pawn_type {
	PEASANT, KING, KNIGHT
};

/**
 * Pawn struct representing single pawn on board.
 */
struct pawn {
	enum pawn_type type;
	int x;
	int y;
	enum player player;
	int last_move_turn;
};

/**
 * Creates new pawn with given attributes.
 */
struct pawn* create_pawn(int x, int y, enum pawn_type type, enum player player, int current_turn);

#endif //PAWN_H
