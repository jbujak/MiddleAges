/** @file
 *  Implementation of pawn management.
 */

#include <stdlib.h>

#include "pawn.h"

struct pawn* create_pawn(int x, int y, enum pawn_type type, enum player player, int current_turn) {
	struct pawn *result;

	result = malloc(sizeof(struct pawn));
	result->x = x;
	result->y = y;
	result->type = type;
	result->player = player;
	result->last_move_turn = current_turn - 1;

	return result;
}
