#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"
#include "engine.h"

int main() {
	int ret;

	start_game();

	struct command *cmd = malloc(sizeof(struct command));
	while (1) {
		ret = parse_command(cmd);

		if(ret == PARSE_END) {
			break;
		}

		switch (cmd->type) {
			case INIT:
				init(cmd->n, cmd->k, cmd->p, cmd->x1, cmd->y1, cmd->x2, cmd->y2);
				break;
			case MOVE:
				move(cmd->x1, cmd->y1, cmd->x2, cmd->y2);
				break;
			case PRODUCE_KNIGHT:
				produce_knight(cmd->x1, cmd->y1, cmd->x2, cmd->y2);
				break;
			case PRODUCE_PEASANT:
				produce_peasant(cmd->x1, cmd->y1, cmd->x2, cmd->y2);
				break;
			case END_TURN:
				end_turn();
				break;
		}

		print_topleft();
	}

	end_game();

	return 0;
}
