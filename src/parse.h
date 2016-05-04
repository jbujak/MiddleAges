 /** @file
  *  Interface of parser.
  */

#ifndef PARSE_H
#define PARSE_H

#define PARSE_OK 0
#define PARSE_END 1
#define PARSE_ERROR 2

#include "global.h"

/**
 * Recognized commands.
 */
enum command_type {
	INIT, MOVE, PRODUCE_KNIGHT, PRODUCE_PEASANT, END_TURN
};

/*
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

/** Reads a command.
 * Fills command structure given in parameter with next command data.
 * @param[out] cmd Command structure to be filled with data.
 * @return PARSE_OK if parsed correctly, PARSE_END if end of file reached or PARSE_ERROR in case of error.
  */
int parse_command(struct command *cmd);

#endif /* PARSE_H */
