/** @file
 *  Main file which coordinates program.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"
#include "parse.h"
#include "command.h"
#include "engine.h"

#define WIN_CODE 0
#define DRAW_CODE 1
#define LOSE_CODE 2
#define INPUT_ERROR_CODE 42

#define OK 0

#define INPUT_ERROR_MSG "input error\n"
#define DRAW_MSG "draw\n"
#define WIN_MSG "%s won\n"

static int main_loop();

/**
 * Processes input until reaches the end of file or game ends.
 * @return OK if succesfully ended, INPUT_ERROR_CODE otherwise.
 */
int main() {
	int ret;
	enum player winner;

	start_game();
	ret = main_loop();
	end_game();

	if(ret != OK) {
		return 	INPUT_ERROR_CODE;
	}

	if(get_current_state() == FINISHED_WON) {
		winner = get_winner();
		if(winner == get_ai_player())
			return WIN_CODE;
		else
			return LOSE_CODE;
	} else {
		return DRAW_CODE;
	}
}

static int main_loop() {
	int ret;
	struct command *cmd;
	bool initialized;

	cmd = malloc(sizeof(struct command));
	initialized = false;

	while (get_current_state() == IN_PROGRESS) {
		if(get_current_player() == get_ai_player() && initialized) {
			make_ai_move();
		} else {
			initialized = true;
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
		}
	}

	free(cmd);

	return OK;
}

