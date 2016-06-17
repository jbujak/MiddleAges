#include <string.h>
#include <stdio.h>

#include "command.h"
#include "utils.h"

void command_to_string(const struct command *cmd, char *buf) {

	switch(cmd->type) {
		case INIT:
			sprintf(buf, "INIT %d %d %d %d %d %d %d",
				cmd->size, cmd->turns, cmd->player,
				cmd->x1, cmd->y1, cmd->x2, cmd->y2);
			break;
		case MOVE:
			sprintf(buf, "MOVE %d %d %d %d",
				cmd->x1, cmd->y1, cmd->x2, cmd->y2);
			break;
		case PRODUCE_KNIGHT:
			sprintf(buf, "PRODUCE_KNIGHT %d %d %d %d",
				cmd->x1, cmd->y1, cmd->x2, cmd->y2);
			break;
		case PRODUCE_PEASANT:
			sprintf(buf, "PRODUCE_PEASANT %d %d %d %d",
				cmd->x1, cmd->y1, cmd->x2, cmd->y2);
			break;
		case END_TURN:
			sprintf(buf, "END_TURN");
			break;
		case NONE:
			strcpy(buf, "");
			break;
	}
}
