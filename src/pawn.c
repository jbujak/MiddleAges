#include <stdlib.h>

#include "pawn.h"

struct pawn* create_pawn(int x, int y, enum pawn_type type, int player) {
	struct pawn *result;

	result = malloc(sizeof(struct pawn));
	result->x = x;
	result->y = y;
	result->type = type;
	result->player = player;

	return result;
}
