#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "list.h"
#include "pawn.h"
#include "engine.h"
#include "preview.h"
#include "global.h"

static int board_size;
static int number_of_turns;

static int current_player;

static enum state current_state = NOT_STARTED;
static int winner;

static struct list *list;

static void add_start_pawns_to_list(int x1, int y1, int x2, int y2);
static void add_player_pawns_to_list(int player, int x, int y);

static void fight(struct pawn *pawn_1, struct pawn *pawn_2, struct pawn **alive_pawn);
static int compare_pawns(struct pawn *pawn_1, struct pawn *pawn_2);

static int next_player();

void start_game() {
	current_player = PLAYER_1;
	list = create_list();
	current_state = IN_PROGRESS;
}

void end_game() {
	free_list(list);
}

void print_topleft() {
	print_preview();
}

int init(int n, int k, int p, int x1, int y1, int x2, int y2) { 
	board_size = n;
	number_of_turns = k;

	init_preview(n, x1, y1, x2, y2);
	add_start_pawns_to_list(x1, y1, x2, y2);

	return MOVE_OK;
}

int move(int x1, int y1, int x2, int y2) {
	struct pawn *moved_pawn;
	struct pawn *destination_pawn;
	struct pawn *alive_pawn;

	moved_pawn = remove_pawn(x1, y1, list);
	destination_pawn = remove_pawn(x2, y2, list);

	moved_pawn->x = x2;
	moved_pawn->y = y2;

	update_preview_remove(x1, y1);
	update_preview_remove(x2, y2);

	fight(moved_pawn, destination_pawn, &alive_pawn);
	if(alive_pawn != NULL) {
		add_pawn(alive_pawn, list);
		update_preview_add(alive_pawn->player, x2, y2, alive_pawn->type);
	}

	return MOVE_OK;
}

int produce_knight(int x1, int y1, int x2, int y2) {
	struct pawn *new_pawn;

	new_pawn = create_pawn(x2, y2, KNIGHT, current_player);
	add_pawn(new_pawn, list);

	update_preview_add(current_player, x2, y2, KNIGHT);

	return MOVE_OK;
}

int produce_peasant(int x1, int y1, int x2, int y2) {
	struct pawn *new_pawn;

	new_pawn = create_pawn(x2, y2, PEASANT, current_player);
	add_pawn(new_pawn, list);

	update_preview_add(current_player, x2, y2, PEASANT);

	return MOVE_OK;
}

int end_turn() {
	current_player = next_player();
	return MOVE_OK;
}

enum state get_current_state() {
	return current_state;
}

static void add_start_pawns_to_list(int x1, int y1, int x2, int y2) {
	add_player_pawns_to_list(x1, y1, PLAYER_1);
	add_player_pawns_to_list(x2, y2, PLAYER_2);
}

static void add_player_pawns_to_list(int x, int y, int player) {
	struct pawn *pawn;

	pawn = create_pawn(x, y, KING, player);
	add_pawn(pawn, list);
	update_preview_add(player, x, y, KING);
	x++;
	
	for(int i = 0; i < INIT_PEASANTS; i++) {
		pawn = create_pawn(x, y, PEASANT, player);
		add_pawn(pawn, list);
		update_preview_add(player, x, y, PEASANT);
		x++;
	}

	for(int i = 0; i < INIT_KNIGHTS; i++) {
		pawn = create_pawn(x, y, KNIGHT, player);
		add_pawn(pawn, list);
		update_preview_add(player, x, y, KNIGHT);
		x++;
	}
}

static void fight(struct pawn *pawn_1, struct pawn *pawn_2, struct pawn **alive_pawn) {
	int cmp;

	if(pawn_2 == NULL) {
		*alive_pawn = pawn_1;
		return;
	}

	cmp = compare_pawns(pawn_1, pawn_2);

	if(cmp > 0) {
		if(pawn_2->type == KING) {
			current_state = FINISHED_WON;
			winner = pawn_1->player;
		}
			
		free(pawn_2);
		*alive_pawn = pawn_1;

	} else if(cmp < 0) {
		if(pawn_1->type == KING) {
			current_state = FINISHED_WON;
			winner = pawn_2->player;
		}

		free(pawn_1);
		*alive_pawn = pawn_2;
	} else { // cmp == 0
		if(pawn_1->type == KING) {
			current_state = FINISHED_DRAW;
		}

		free(pawn_1);
		free(pawn_2);
		*alive_pawn = NULL;
	}
}

static int compare_pawns(struct pawn *pawn_1, struct pawn *pawn_2) {
	return pawn_1->type - pawn_2 -> type;
}

static int next_player() {
	return current_player == PLAYER_1 ? PLAYER_2 : PLAYER_1;
}
