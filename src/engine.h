/** @file
    Interface of game engine.

 */

#ifndef ENGINE_H
#define ENGINE_H

#define MOVE_OK 0
#define MOVE_INVALID 1

enum state {
	NOT_STARTED, IN_PROGRESS, FINISHED_WON, FINISHED_DRAW
};

/**
 * Initializes a game. Needed before first INIT.
 */
void start_game();

/**
 * Frees memory. Needed after finishing game.
 */
void end_game();

/**
 * Initializes a game with size of a board, number of rounds and positions of kings.
 */
int init(int n, int k, int p, int x1, int y1, int x2, int y2);

/**
 * Makes a move.
 * @param[in] x1 Column number before a move.
 * @param[in] y1 Row number before a move.
 * @param[in] x2 Column number after a move.
 * @param[in] y2 Row number after a move.
 * @return 0.
 */
int move(int x1, int y1, int x2, int y2);

int produce_knight(int x1, int y1, int x2, int y2);

int produce_peasant(int x1, int y1, int x2, int y2);

int end_turn();

/**
 * Prints (into stdout) top-left corner of the board of size m x m where m = min(n,10).
 */
void print_topleft();

enum state get_current_state();

#endif /* ENGINE_H */
