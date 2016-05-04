/** @file
 *  Main file which coordinates program.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"
#include "engine.h"

#define INPUT_ERROR_CODE 42
#define OK 0

#define INPUT_ERROR_MSG "input error\n"
#define DRAW_MSG "draw\n"
#define WIN_MSG "%s won\n"

static int main_loop();
static int execute_command(struct command *cmd);
static void print_result();
static char* player_to_string(enum player player);

/**
 * Processes input until reaches the end of file or game ends.
 * @return OK if succesfully ended, INPUT_ERROR_CODE otherwise.
 */
int main() {
	int ret;

	start_game();
	ret = main_loop();
	end_game();

	if(ret != OK) {
		fprintf(stderr, INPUT_ERROR_MSG);
		return 	INPUT_ERROR_CODE;
	}
	
	print_result();

	return OK;
}

static int main_loop() {
	int ret;
	struct command *cmd;

	cmd = malloc(sizeof(struct command));

	while (get_current_state() == IN_PROGRESS) {
		ret = parse_command(cmd);
		if(ret == PARSE_END) {
			break;
		} else if(ret == PARSE_ERROR) {
			free(cmd);
			return INPUT_ERROR_CODE;
		}

		ret = execute_command(cmd);

		if(ret != MOVE_OK) {
			free(cmd);
			return INPUT_ERROR_CODE;
		}

		if(cmd->type != END_TURN) {
			print_topleft();
			}
	}

	free(cmd);

	return OK;
}

static int execute_command(struct command *cmd) {
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
		default:
			return MOVE_INVALID;
	}

}

static void print_result() {
	enum player winner;
	if(get_current_state() == FINISHED_WON) {
		winner = get_winner();
		fprintf(stderr, WIN_MSG, player_to_string(winner));
	} else {
		fprintf(stderr, DRAW_MSG);
	}
}

static char* player_to_string(enum player player) {
	switch(player) {
		case PLAYER_1:
			return "player 1";
		case PLAYER_2:
			return "player 2";
		default:
			return "";
	}
}
