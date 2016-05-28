 /** @file
  *  Implementation of parser.
  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "parse.h"

#define LINE_MAX 100

#define INIT_CMD "INIT"
#define MOVE_CMD "MOVE"
#define PRODUCE_KNIGHT_CMD "PRODUCE_KNIGHT"
#define PRODUCE_PEASANT_CMD "PRODUCE_PEASANT"
#define END_TURN_CMD "END_TURN"

#define PLAYER_1_INT 1
#define PLAYER_2_INT 2

#define COMMAND_POSITION 0

#define INIT_SIZE_POSITION 1
#define INIT_TURNS_POSITION 2
#define INIT_PLAYER_POSITION 3
#define INIT_X_Y_POSITION 4
#define INIT_LAST_POSITION 7

#define MOVE_ACTION_X_Y_POSITION 1
#define MOVE_ACTION_LAST_POSITION 4

#define END_TURN_LAST_POSITION 0

#define X1_OFFSET 0
#define Y1_OFFSET 1
#define X2_OFFSET 2
#define Y2_OFFSET 3

static int process_line(const char *line, struct command *cmd);

// Declaration of functions for filling command struct.
static int fill_init_struct(const char *line, struct command *cmd);
static int fill_move_action_struct(const char *line, struct command *cmd, enum command_type type);
static int fill_end_turn_struct(const char *line, struct command *cmd);
static int fill_x_y(int begin, const char *line, struct command *cmd);

// Declaration of functions for manging tokens.
static void get_token(int n, const char *line, char *buf);
static int number_of_tokens(const char *line);
static int get_number(int n, const char *line, int *error);
static bool is_number(const char *str);


// Implementation of exported function

int parse_command(struct command *cmd) {
	char *line;
	int ret;
	
	line = malloc(LINE_MAX * sizeof(char));

	fgets(line, LINE_MAX, stdin);

	if(feof(stdin)) {
		free(line);
		return PARSE_END;
	}

	ret = process_line(line, cmd);

	free(line);

	return ret;
}

static int process_line(const char *line, struct command *cmd) {
	int ret;
	char *command_str;

	ret = PARSE_OK;
	command_str = malloc(LINE_MAX * sizeof(char));

	get_token(COMMAND_POSITION, line, command_str);

	if(strcmp(command_str, INIT_CMD) == 0)
		ret = fill_init_struct(line, cmd);
	else if(strcmp(command_str, MOVE_CMD) == 0)
		ret = fill_move_action_struct(line, cmd, MOVE);
	else if(strcmp(command_str, PRODUCE_KNIGHT_CMD) == 0)
		ret = fill_move_action_struct(line, cmd, PRODUCE_KNIGHT);
	else if(strcmp(command_str, PRODUCE_PEASANT_CMD) == 0)
		ret = fill_move_action_struct(line, cmd, PRODUCE_PEASANT);
	else if(strcmp(command_str, END_TURN_CMD) == 0)
		ret = fill_end_turn_struct(line, cmd);
	else
		ret = PARSE_ERROR;

	free(command_str);

	return ret;
}

// Implementation of functions for filling command struct

static int fill_init_struct(const char *line, struct command *cmd) {
	int ret;
	int player_int;

	ret = PARSE_OK;

	if(number_of_tokens(line) != INIT_LAST_POSITION)
		return PARSE_ERROR;

	cmd->type = INIT;
	cmd->size = get_number(INIT_SIZE_POSITION, line, &ret);
	cmd->turns = get_number(INIT_TURNS_POSITION, line, &ret);

	player_int = get_number(INIT_PLAYER_POSITION, line, &ret);
	if(ret != PARSE_OK)
		return ret;

	switch(player_int) {
		case PLAYER_1_INT:
			cmd->player = PLAYER_1;
			break;
		case PLAYER_2_INT:
			cmd->player = PLAYER_2;
			break;
		default:
			return PARSE_ERROR;
	}

	ret =  fill_x_y(INIT_X_Y_POSITION, line, cmd);

	return ret;
}

static int fill_move_action_struct(const char *line, struct command *cmd, enum command_type type) {
	if(number_of_tokens(line) != MOVE_ACTION_LAST_POSITION)
		return PARSE_ERROR;

	cmd->type = type;

	return fill_x_y(MOVE_ACTION_X_Y_POSITION, line, cmd);
}

static int fill_end_turn_struct(const char *line, struct command *cmd) {
	if(number_of_tokens(line) != END_TURN_LAST_POSITION)
		return PARSE_ERROR;

	cmd->type = END_TURN;

	return PARSE_OK;
}

static int fill_x_y(int begin, const char *line, struct command *cmd) {
	int ret = PARSE_OK;

	cmd->x1 = get_number(begin + X1_OFFSET, line, &ret);
	cmd->y1 = get_number(begin + Y1_OFFSET, line, &ret);
	cmd->x2 = get_number(begin + X2_OFFSET, line, &ret);
	cmd->y2 = get_number(begin + Y2_OFFSET, line, &ret);

	return ret;
}

// Implementation of functions for manging tokens.

static void get_token(int pos, const char *line, char *buf) {
	const char *current = line;
	int token_number = 0;
	int i = 0;

	while(token_number <= pos) {
		i = 0;
		while((*current != ' ' && *current != '\n') && *current != '\0') {
			buf[i] = *current;
			current++;
			i++;
		}
		if(*current == '\n' && token_number < pos) {
			*buf = '\0';
			return;
		}
		token_number++;
		current++;
	}

	buf[i] = '\0';
}

static int number_of_tokens(const char *line) {
	int res;
	const char *current;

	res = 0;
	current = line;
	while(*current != '\n' && *current != '\0') {
		current++;
		if(*current == ' ')
			res++;
	}

	return res;
}

static int get_number(int pos, const char *line, int *error) {
	char *buf;
	int result;
	
	buf = malloc(LINE_MAX * sizeof(char));
	
	get_token(pos, line, buf);
	if(is_number(buf))
		result = atoi(buf);
	else
		*error = PARSE_ERROR;

	free(buf);

	return result;
}

static bool is_number(const char *str) {
	if(str == NULL)
		return false;
	if(*str == '\0')
		return false;

	while(*str != '\0') {
		if(!isdigit(*str))
			return false;
		str++;
	}

	return true;
}
