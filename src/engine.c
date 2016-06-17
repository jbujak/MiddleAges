/** @file
 *  Implementation of game engine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "ai.h"
#include "list.h"
#include "pawn.h"
#include "engine.h"
#include "preview.h"
#include "global.h"
#include "utils.h"

#define PAWN_RANGE 1
#define DELAY_BEFORE_ACTION 2

#define LINE_MAX 100

static const enum player START_PLAYER = PLAYER_1;

static int board_size;
static int number_of_turns;

static enum state current_state = NOT_STARTED;
static int current_turn;
static enum player current_player;
static enum player winner;

static int init_counter;
static enum player first_init_player;

static enum player ai_player;

static struct list *list;

// Declaration of functions used during initialization.
static void add_start_pawns_to_list(int x, int y, enum player player);

// Declaration of functions used during move and action.
static int produce_pawn(int x1, int y1, int x2, int y2, enum pawn_type pawn_type);
static bool is_action_valid(int x1, int y1, int x2, int y2);
static bool is_move_valid(int x1, int y1, int x2, int y2);

// Declaration of functions used during fight.
static void fight(struct pawn *pawn_1, struct pawn *pawn_2, struct pawn **alive_pawn);
static int compare_pawns(struct pawn *pawn_1, struct pawn *pawn_2);

// Declaration of function used on end of turn.
static int next_player();


// Implementation of exported functions.

void start_game() {
	winner = NONE_PLAYER;
	current_player = START_PLAYER;
	current_state = IN_PROGRESS;

	current_turn = 1;
	init_counter = 0;

	list = create_list();
}

int end_game() {
	if(init_counter != 1)
		return MOVE_INVALID;

	clean_ai();
	free_list(list);
	if(current_state != FINISHED_WON)
		current_state = FINISHED_DRAW;

	return MOVE_OK;
}

void print_topleft() {
	print_preview();
}

int init(int size, int turns, enum player player, int x1, int y1, int x2, int y2) { 
	init_counter++;

	if(init_counter == 1) {
		// First call of init.

		board_size = size;
		number_of_turns = turns;

		init_preview(size);
		add_start_pawns_to_list(x1, y1, PLAYER_1);
		add_start_pawns_to_list(x2, y2, PLAYER_2);
		first_init_player = player;
		ai_player = player;

		if(ai_player == PLAYER_1)
			init_ai(x1, y1, x2, y2, list);
		else
			init_ai(x2, y2, x1, y1, list);

	} else {
		// More than one init - error.

		return MOVE_INVALID;
	}

	return MOVE_OK;
}

int move(int x1, int y1, int x2, int y2) {
	if(init_counter != 1)
		return MOVE_INVALID;

	struct pawn *source_pawn;
	struct pawn *destination_pawn;
	struct pawn *alive_pawn;


	if(!is_move_valid(x1, y1, x2, y2))
		return MOVE_INVALID;

	source_pawn = remove_pawn(x1, y1, list);
	destination_pawn = remove_pawn(x2, y2, list);

	source_pawn->x = x2;
	source_pawn->y = y2;
	source_pawn->last_move_turn = current_turn;

	update_preview_remove(x1, y1);
	update_preview_remove(x2, y2);

	fight(source_pawn, destination_pawn, &alive_pawn);
	if(alive_pawn != NULL) {
		add_pawn(alive_pawn, list);
		update_preview_add(alive_pawn->player, x2, y2, alive_pawn->type);
	}

	return MOVE_OK;
}

int produce_knight(int x1, int y1, int x2, int y2) {
	if(init_counter != 1)
		return MOVE_INVALID;

	return produce_pawn(x1, y1, x2, y2, KNIGHT);
}

int produce_peasant(int x1, int y1, int x2, int y2) {
	if(init_counter != 1)
		return MOVE_INVALID;

	return produce_pawn(x1, y1, x2, y2, PEASANT);
}

int end_turn() {
	if(init_counter != 1)
		return MOVE_INVALID;

	current_player = next_player();
	if(current_player == PLAYER_1) {
		current_turn++;
		if(current_turn > number_of_turns)
			current_state = FINISHED_DRAW;
	}

	return MOVE_OK;
}

enum player get_winner() {
	return winner;
}

enum player get_current_player() {
	return current_player;
}

enum state get_current_state() {
	return current_state;
}

enum player get_ai_player() {
	return ai_player;
}

void make_ai_move() {
	struct command *command;
	char *move;
	
	command = malloc(sizeof(struct command));
	move = malloc(LINE_MAX * sizeof(char));

	do {
		get_ai_move(command);
		if(command->type == NONE)
			continue;

		command_to_string(command, move);

		printf("%s\n", move);
		fflush(stdout);
		execute_command(command);
	} while(command->type != END_TURN && current_state == IN_PROGRESS);


	free(command);
	free(move);
}

int execute_command(struct command *cmd) {
	switch (cmd->type) {
		case INIT:
			return init(cmd->size, cmd->turns, cmd->player,
				cmd->x1, cmd->y1, cmd->x2, cmd->y2);
		case MOVE:
			return move(cmd->x1, cmd->y1, cmd->x2, cmd->y2);
		case PRODUCE_KNIGHT:
			return produce_knight(cmd->x1, cmd->y1, cmd->x2, cmd->y2);
		case PRODUCE_PEASANT:
			return produce_peasant(cmd->x1, cmd->y1, cmd->x2, cmd->y2);
		case END_TURN:
			return end_turn();
		case NONE:
			return MOVE_OK;
		default:
			return MOVE_INVALID;
	}

}

// Implementation of functions used during initialization.

static void add_start_pawns_to_list(int x, int y, enum player player) {
	struct pawn *pawn;

	pawn = create_pawn(x, y, KING, player, current_turn);
	add_pawn(pawn, list);
	update_preview_add(player, x, y, KING);
	x++;
	
	for(int i = 0; i < INIT_PEASANTS; i++) {
		pawn = create_pawn(x, y, PEASANT, player, current_turn);
		add_pawn(pawn, list);
		update_preview_add(player, x, y, PEASANT);
		x++;
	}

	for(int i = 0; i < INIT_KNIGHTS; i++) {
		pawn = create_pawn(x, y, KNIGHT, player, current_turn);
		add_pawn(pawn, list);
		update_preview_add(player, x, y, KNIGHT);
		x++;
	}
}


// Implementation of functions used during move and action.

static int produce_pawn(int x1, int y1, int x2, int y2, enum pawn_type pawn_type) {
	struct pawn *source_pawn;
	struct pawn *new_pawn;

	if(!is_action_valid(x1, y1, x2, y2))
		return MOVE_INVALID;

	source_pawn = get_pawn(x1, y1, list);
	source_pawn->last_move_turn = current_turn;

	new_pawn = create_pawn(x2, y2, pawn_type, current_player, current_turn);
	add_pawn(new_pawn, list);
	if(new_pawn->player == get_ai_player())
		add_ai_pawn(new_pawn);

	update_preview_add(current_player, x2, y2, pawn_type);

	return MOVE_OK;
}

static bool is_action_valid(int x1, int y1, int x2, int y2) {
	struct pawn *source_pawn;
	struct pawn *destination_pawn;

	if(abs(x1 - x2) > PAWN_RANGE || abs(y1 - y2) > PAWN_RANGE)
		return false;

	source_pawn = get_pawn(x1, y1, list);
	if(source_pawn == NULL)
		return false;
	if(source_pawn->player != current_player)
		return false;
	if(source_pawn->type != PEASANT)
		return false;
	if(current_turn - source_pawn->last_move_turn <= DELAY_BEFORE_ACTION)
		return false;

	destination_pawn = get_pawn(x2, y2, list);
	if(destination_pawn != NULL)
		return false;


	return true;
}

static bool is_move_valid(int x1, int y1, int x2, int y2) {
	struct pawn *source_pawn;
	struct pawn *destination_pawn;

	if(abs(x1 - x2) > PAWN_RANGE || abs(y1 - y2) > PAWN_RANGE)
		return false;

	if(x2 < 1 || x2 > board_size)
		return false;
	if(y2 < 1 || y2 > board_size)
		return false;

	source_pawn = get_pawn(x1, y1, list);
	if(source_pawn == NULL)
		return false;
	if(source_pawn->last_move_turn == current_turn)
		return false;
	if(source_pawn->player != current_player)
		return false;

	destination_pawn = get_pawn(x2, y2, list);
	if(destination_pawn == NULL)
		return true;
	if(destination_pawn->player == source_pawn->player)
		return false;

	return true;
}

// Implementation of functions used during fight.

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
			
		if(pawn_2->player == get_ai_player())
			remove_ai_pawn(pawn_2);

		free(pawn_2);

		*alive_pawn = pawn_1;
	} else if(cmp < 0) {
		if(pawn_1->type == KING) {
			current_state = FINISHED_WON;
			winner = pawn_2->player;
		}

		if(pawn_1->player == get_ai_player())
			remove_ai_pawn(pawn_1);

		free(pawn_1);

		*alive_pawn = pawn_2;
	} else { 
		if(pawn_1->type == KING)
			current_state = FINISHED_DRAW;

		if(pawn_1->player == get_ai_player())
			remove_ai_pawn(pawn_1);
		if(pawn_2->player == get_ai_player())
			remove_ai_pawn(pawn_2);

		free(pawn_1);
		free(pawn_2);

		*alive_pawn = NULL;
	}
}

static int compare_pawns(struct pawn *pawn_1, struct pawn *pawn_2) {
	return pawn_1->type - pawn_2->type;
}

// Implementation of function used on end of turn.

static int next_player() {
	return current_player == PLAYER_1 ? PLAYER_2 : PLAYER_1;
}
