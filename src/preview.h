#ifndef PREVIEW_H
#define PREVIEW_H

#include "pawn.h"

void init_preview(int n, int x1, int y1, int x2, int y2);

void update_preview_add(int player, int x, int y, enum pawn_type pawn_type);

void update_preview_remove(int x, int y);

void print_preview();

#endif //PREVIEW_H
