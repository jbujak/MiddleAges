#include <stdio.h>

#include "preview.h"
#include "pawn.h"
#include "global.h"

#define MAX_PREVIEW_SIZE 10
#define PREVIEW_EMPTY '.'
#define PREVIEW_KING 'k'
#define PREVIEW_KNIGHT 'r'
#define PREVIEW_PEASANT 'c'

int preview_size;
static char preview[MAX_PREVIEW_SIZE + 1][MAX_PREVIEW_SIZE + 1];

static char get_preview_symbol(int player, enum pawn_type pawn_type);

static char upcase(const char c);
static char downcase(const char c);

static void fill_preview();

void init_preview(int n, int x1, int y1, int x2, int y2) {
	preview_size = n < MAX_PREVIEW_SIZE ? n : MAX_PREVIEW_SIZE;
	fill_preview();
}

void update_preview_add(int player, int x, int y, enum pawn_type pawn_type) {
	if(x > MAX_PREVIEW_SIZE || y > MAX_PREVIEW_SIZE) 
		return;

	preview[x][y] = get_preview_symbol(player, pawn_type);
}

void update_preview_remove(int x, int y) {
	if(x > MAX_PREVIEW_SIZE || y > MAX_PREVIEW_SIZE)
		return;

	preview[x][y] = PREVIEW_EMPTY;
}

void print_preview() {
	for(int row = 1; row <= preview_size; row++) {
		for(int column = 1; column <= preview_size; column++) {
			printf("%c", preview[column][row]);
		}
		printf("\n");
	}
	printf("\n");

}

static char get_preview_symbol(int player, enum pawn_type pawn_type) {
	char result;
	switch(pawn_type) {
		case PEASANT:
			result = PREVIEW_PEASANT;
			break;
		case KNIGHT:
			result = PREVIEW_KNIGHT;
			break;
		case KING: 
			result = PREVIEW_KING;
	}

	return player == PLAYER_1 ? upcase(result) : downcase(result);
}

static char upcase(const char c) {
	return c & (~(1 << 5));
}

static char downcase(const char c) {
	return c | (1 << 5);
}

static void fill_preview() {
	for(int i = 1; i <= preview_size; i++) {
		for(int j = 1; j <= preview_size; j++) {
			preview[i][j] = PREVIEW_EMPTY;
		}
	}
}
