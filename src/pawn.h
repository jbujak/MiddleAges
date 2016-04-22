#ifndef PAWN_H
#define PAWN_H

// Pawn types are ordered by increasing power
enum pawn_type {
	PEASANT, KING, KNIGHT
};

struct pawn {
	enum pawn_type type;
	int x;
	int y;
	int player;
};

struct pawn* create_pawn(int x, int y, enum pawn_type type, int player);

#endif //PAWN_H
