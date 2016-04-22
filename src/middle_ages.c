#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"
#include "engine.h"

#define INPUT_ERROR_CODE 42
#define INPUT_ERROR_MSG "input error"

int main() {
	int ret;

	int counter = 0;

	start_game();

	struct command *cmd = malloc(sizeof(struct command));
	while (get_current_state() == IN_PROGRESS) {
		ret = parse_command(cmd);
		if(ret == PARSE_END) {
			break;
		} else if(ret == PARSE_ERROR) {
			fprintf(stderr, INPUT_ERROR_MSG);
			return INPUT_ERROR_CODE;
		}

		switch (cmd->type) {
			case INIT:
				ret = init(cmd->n, cmd->k, cmd->p, cmd->x1, cmd->y1, cmd->x2, cmd->y2);
				break;
			case MOVE:
				ret = move(cmd->x1, cmd->y1, cmd->x2, cmd->y2);
				break;
			case PRODUCE_KNIGHT:
				ret = produce_knight(cmd->x1, cmd->y1, cmd->x2, cmd->y2);
				break;
			case PRODUCE_PEASANT:
				ret = produce_peasant(cmd->x1, cmd->y1, cmd->x2, cmd->y2);
				break;
			case END_TURN:
				ret = end_turn();
				counter++;
				break;
		}

		if(ret != MOVE_OK) {
			fprintf(stderr, INPUT_ERROR_MSG);
			return INPUT_ERROR_CODE;
		}

		if(cmd->type != END_TURN) {
			print_topleft();
		}
	}

	end_game();
	free(cmd);

	return 0;
}
