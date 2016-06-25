#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "ai.h"
#include "list.h"
#include "command.h"
#include "engine.h"
#include "pawn.h"
#include "utils.h"

static bool can_produce_peasant;
static bool can_produce_knight_1;
static bool can_produce_knight_2;

static int current_turn;
static int current_move;

static struct pawn *current_knight;
static struct pawn *peasant_1;
static struct pawn *peasant_2;
static struct pawn *enemy_king;

static struct list* knights;
static struct list* pawns;


/* Function called on start of each turn */
static void init_move();

/* Functions for making different kinds of moves */
static void place_peasant(struct command *command);
static void place_knight(struct pawn *peasant, struct command *command);
static void move_knight(struct command *command);
static void ai_end_turn(struct command *command);

/* Helper function for searching nearest enemy */
static struct pawn* get_nearest_enemy_pawn(int x, int y);

void init_ai(int x, int y, int enemy_x, int enemy_y, struct list *pawn_list) {
	struct pawn *knight;

	pawns = pawn_list;
	knights = create_list();

	enemy_king = get_pawn(enemy_x, enemy_y, pawns);

	peasant_1 = get_pawn(x + 1, y, pawns);
	peasant_2 = malloc(sizeof(struct pawn));

	knight = get_pawn(x + 2, y, pawns);
	add_pawn(knight, knights);

	knight = get_pawn(x + 3, y, pawns);
	add_pawn(knight, knights);

	current_turn = 1;
	current_move = 0;
}

void clean_ai() {
	free_list_wihout_content(knights);
	free(peasant_2);
}

void get_ai_move(struct command *command) {
	current_move++;

	if(current_move == 1)
		init_move();
	
	if(can_produce_peasant)
		place_peasant(command);
	else if(can_produce_knight_1)
		place_knight(peasant_1, command);
	else if(can_produce_knight_2)
		place_knight(peasant_2, command);
	else if(current_knight != NULL)
		move_knight(command);
	else
		ai_end_turn(command);
}

void add_ai_pawn(struct pawn *pawn) {
	if(pawn->type == KNIGHT)
		add_pawn(pawn, knights);
}

void remove_ai_pawn(struct pawn *pawn) {
	if(pawn->type == KNIGHT)
		remove_pawn(pawn->x, pawn->y, knights);
}


static void init_move() {
	current_knight = get_first_pawn(knights);

	if(current_turn < 3) {
		can_produce_peasant = false;
		can_produce_knight_1 = false;
		can_produce_knight_2 = false;
	} else if(current_turn == 3) {
		can_produce_peasant = true;
		can_produce_knight_1 = false;
		can_produce_knight_2 = false;
	} else {
		can_produce_peasant = false;
		can_produce_knight_1 = (current_turn % 3 == 0);
		can_produce_knight_2 = (current_turn % 3 == 0);
	}
}

static void place_peasant(struct command *command) {
	int x2;
	int y2;

	if(enemy_king->x > peasant_1->x)
		x2 = peasant_1->x - 1;
	else
		x2 = peasant_1->x + 1;

	if(enemy_king->y > peasant_1->y)
		y2 = peasant_1->y + 1;
	else
		y2 = peasant_1->y - 1;
	
	if(y2 > get_board_size())
		y2 -= 2;
	else if(y2 < 1)
		y2 += 2;

	command->type = PRODUCE_PEASANT;

	command->x1 = peasant_1->x;
	command->y1 = peasant_1->y;

	command->x2 = x2;
	command->y2 = y2;

	peasant_2->x = x2;
	peasant_2->y = y2;

	can_produce_peasant = false;
}

static void place_knight(struct pawn *peasant, struct command *command) {
	command->type = PRODUCE_KNIGHT;

	command->x1 = peasant->x;
	command->y1 = peasant->y;

	command->x2 = peasant->x;
	command->y2 = peasant->y;

	if(enemy_king->y > peasant->y)
		command->y2++;
	else
		command->y2--;

	if(command->y2 > get_board_size())
		command->y2 -= 2;
	else if(command->y2 < 1)
		command->y2 += 2;

	if(get_pawn(command->x2, command->y2, pawns) != NULL) {
		command->type = NONE;
	}

	if(can_produce_knight_1)
		can_produce_knight_1 = false;
	else
		can_produce_knight_2 = false;
	
	current_knight = get_first_pawn(knights);
}

static void move_knight(struct command *command) {
	struct pawn *target;
	struct pawn *dest_pawn;
	int x2[3];
	int y2[3];

	target = get_nearest_enemy_pawn(current_knight->x, current_knight->y);

	if(target->x > current_knight->x) {
		x2[0] = current_knight->x + 1;
		x2[1] = current_knight->x;
		x2[2] = current_knight->x;
	}
	else if(target->x < current_knight->x) {
		x2[0] = current_knight->x - 1;
		x2[1] = current_knight->x;
		x2[2] = current_knight->x;
	}
	else {
		x2[0] = current_knight->x;
		x2[1] = current_knight->x - 1;
		x2[2] = current_knight->x + 1;
	}

	if(target->y > current_knight->y) {
		y2[0] = current_knight->y + 1;
		y2[1] = current_knight->y;
		y2[2] = current_knight->y;
	}
	else if(target->y < current_knight->y) {
		y2[0] = current_knight->y - 1;
		y2[1] = current_knight->y;
		y2[2] = current_knight->y;
	}
	else {
		y2[0] = current_knight->y;
		y2[1] = current_knight->y - 1;
		y2[2] = current_knight->y + 1;
	}


	command->type = MOVE;

	command->x1 = current_knight->x;
	command->y1 = current_knight->y;

	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			if(x2[i] < 1 || x2[j] > get_board_size()) continue;
			if(y2[j] < 1 || y2[j] > get_board_size()) continue;

			dest_pawn = get_pawn(x2[i], y2[j], pawns);
			if(dest_pawn == NULL || dest_pawn->player != get_ai_player()) {
				command->x2 = x2[i];
				command->y2 = y2[j];

				current_knight = get_next_pawn(knights);

				return;
			}
		}
	}

	command->type = NONE;
	current_knight = get_next_pawn(knights);
}

static void ai_end_turn(struct command *command) {
	command->type = END_TURN;

	current_turn++;
	current_move = 0;
}

static struct pawn* get_nearest_enemy_pawn(int x, int y) {
	int current_distance;
	int result_distance;
	struct pawn *result;
	struct pawn *current;

	current = get_first_pawn(pawns);
	result = NULL;
	int enemies = 0;
	while(current != NULL) {
		if(current->player != get_ai_player()) {
			enemies++;
			if(result == NULL) {
				result = current;
			} else {
				current_distance = abs(current->x - x) +
					abs(current->y - y);
				result_distance = abs(result->x - x) +
					abs(result->y - y);

				if(current_distance < result_distance)
					result = current;
				else if(current_distance == result_distance && current->x < result->x)
					result = current;
			}

		}
		current = get_next_pawn(pawns);
	}
	
	return result;
}
