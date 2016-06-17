 /** @file
  *  Interface of parser.
  */

#ifndef PARSE_H
#define PARSE_H

#define PARSE_OK 0
#define PARSE_END 1
#define PARSE_ERROR 2

#include "global.h"
#include "command.h"

/** Reads a command.
 * Fills command structure given in parameter with next command data.
 * @param[out] cmd Command structure to be filled with data.
 * @return PARSE_OK if parsed correctly, PARSE_END if end of file reached or PARSE_ERROR in case of error.
  */
int parse_command(struct command *cmd);

#endif /* PARSE_H */
