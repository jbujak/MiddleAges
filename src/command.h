#ifndef COMMAND_H
#define COMMAND_H
#include "global.h"

/**
 * Recognized commands.
 */
enum command_type {
	INIT, MOVE, PRODUCE_KNIGHT, PRODUCE_PEASANT, END_TURN, NONE
};

/**
 * Structure used to represent single command.
 */
struct command {
	enum command_type type;
	int size;
	int turns;
	enum player player;
	int x1, y1;
	int x2, y2;
};

/**
 * Produces valid command string from command structure
 */
void command_to_string(const struct command *command, char *buf);

#endif //COMMAND_H
