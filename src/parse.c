#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parse.h"

#define LINE_MAX 100

#define INIT_CMD "INIT"
#define MOVE_CMD "MOVE"
#define PRODUCE_KNIGHT_CMD "PRODUCE_KNIGHT"
#define PRODUCE_PEASANT_CMD "PRODUCE_PEASANT"
#define END_TURN_CMD "END_TURN"

#define COMMAND_POSITION 0
#define N_POSITION 1
#define K_POSITION 2
#define P_POSITION 3

#define INIT_X_Y_POSITION 4
#define MOVE_X_Y_POSITION 1
#define PRODUCE_KNIGHT_X_Y_POSITION 1 
#define PRODUCE_PEASANT_X_Y_POSITION 1 

#define X1_OFFSET 0
#define Y1_OFFSET 1
#define X2_OFFSET 2
#define Y2_OFFSET 3

static void fill_x_y(int begin, char *line, struct command *ret);
static int get_number(int n, char *line);
static void get_token(int n, char *line, char *buf);

int parse_command(struct command *ret) {
	char *line;
	char *command_str;
	
	line = malloc(LINE_MAX * sizeof(char));
	command_str = malloc(LINE_MAX * sizeof(char));

	fgets(line, LINE_MAX, stdin);
	if(feof(stdin)) {
		free(command_str);
		free(line);
		return PARSE_END;
	}

	get_token(COMMAND_POSITION, line, command_str);

	if(strcmp(command_str, INIT_CMD) == 0) {
		ret->type = INIT;
		ret->n = get_number(N_POSITION, line);
		ret->k = get_number(K_POSITION, line);
		ret->p = get_number(P_POSITION, line);
		fill_x_y(INIT_X_Y_POSITION, line, ret);
	} else if(strcmp(command_str, MOVE_CMD) == 0) {
		ret->type = MOVE;
		fill_x_y(MOVE_X_Y_POSITION, line, ret);
	} else if(strcmp(command_str, PRODUCE_KNIGHT_CMD) == 0) {
		ret->type = PRODUCE_KNIGHT;
		fill_x_y(PRODUCE_KNIGHT_X_Y_POSITION, line, ret);
	} else if(strcmp(command_str, PRODUCE_PEASANT_CMD) == 0) {
		ret->type = PRODUCE_PEASANT;
		fill_x_y(PRODUCE_PEASANT_X_Y_POSITION, line, ret);
	} else if(strcmp(command_str, END_TURN_CMD) == 0) {
		ret->type = END_TURN;
	} else {
		free(command_str);
		free(line);
		return PARSE_ERROR;
	}

	free(command_str);
	free(line);

	return PARSE_OK;
}

static void fill_x_y(int begin, char *line, struct command *ret) {
	ret->x1 = get_number(begin + X1_OFFSET, line);
	ret->y1 = get_number(begin + Y1_OFFSET, line);
	ret->x2 = get_number(begin + X2_OFFSET, line);
	ret->y2 = get_number(begin + Y2_OFFSET, line);
}

static int get_number(int pos, char *line) {
	char *buf;
	int res;
	
	buf = malloc(LINE_MAX * sizeof(char));
	
	get_token(pos, line, buf);
	res = atoi(buf);

	free(buf);

	return res;
}

static void get_token(int pos, char *line, char *buf) {
	char *current = line;
	int token_number = 0;
	int i = 0;

	while(token_number <= pos) {
		i = 0;
		while(*current != ' ' && *current != '\n') {
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


