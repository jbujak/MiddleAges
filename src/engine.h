/** @file
 *  Interface of game engine.
 */

#ifndef ENGINE_H
#define ENGINE_H

#define MOVE_OK 0
#define MOVE_INVALID 1

#include "command.h"
#include "pawn.h"

/**
 * Possible states of a game
 */
enum state {
	NOT_STARTED, IN_PROGRESS, FINISHED_WON, FINISHED_DRAW
};

/**
 * Initializes a game. Needed before first INIT.
 */
void start_game();

/**
 * Frees memory. Needed after finishing game.
 * @return MOVE_OK if called in appropriate moment, MOVE_INVALID otherwise
 */
int end_game();

/**
 * Initializes a game with size of a board, number of rounds and positions of kings.
 * @param[in] size Size of board.
 * @param[in] turns Maximal number of turns.
 * @param[in] player Number of initialized player.
 * @param[in] x1 Initial column of first player's king.
 * @param[in] y1 Initial row of first player's king.
 * @param[in] x2 Initial column of second player's king.
 * @param[in] y2 Initial row of second player's king.
 * @return MOVE_OK if move was correct or MOVE_INVALID if not.
 */
int init(int size, int turns, enum player player, int x1, int y1, int x2, int y2);

/**
 * Makes a move.
 * @param[in] x1 Column number before a move.
 * @param[in] y1 Row number before a move.
 * @param[in] x2 Column number after a move.
 * @param[in] y2 Row number after a move.
 * @return MOVE_OK if move was correct or MOVE_INVALID if not.
 */
int move(int x1, int y1, int x2, int y2);

/**
 * Creates new knight at a given position
 * @param[in] x1 Column with a peasant able to make action.
 * @param[in] y1 Row with a peasant able to make action.
 * @param[in] x2 Column for new knight.
 * @param[in] y2 Row for new knight.
 * @return MOVE_OK if move was correct or MOVE_INVALID if not.
 */
int produce_knight(int x1, int y1, int x2, int y2);

/**
 * Creates new peasant at a given position.
 * @param[in] x1 Column with a peasant able to make action.
 * @param[in] y1 Row with a peasant able to make action.
 * @param[in] x2 Column for new peasant.
 * @param[in] y2 Row for new peasant.
 * @return MOVE_OK if move was correct or MOVE_INVALID if not.
 */
int produce_peasant(int x1, int y1, int x2, int y2);

/**
 * Ends turn of a player and begins next player turn.
 * @return MOVE_OK
 */
int end_turn();

/**
 * Prints (into stdout) top-left corner of the board of size m x m where m = min(n,10).
 */
void print_topleft();

/**
 * Returns current state of a game.
 * @return Current state of a game.
 */
enum state get_current_state();

/**
 * Returns player who won the game.
 * @return PLAYER_1 or PLAYER_2 if one of players has won, NONE_PLAYER if not.
 */
enum player get_winner();

/**
 * Returns current player.
 */
enum player get_current_player();

/**
 * Return player owned by AI.
 */
enum player get_ai_player();

/**
 * Makes move and prints it on stdout.
 */
void make_ai_move();

/**
 * @return board size
 */
int get_board_size();

/**
 * Executes command given in argument.
 * @return MOVE_OK if move was correct or MOVE_INVALID if not.
 */
int execute_command(struct command *cmd);

#endif /* ENGINE_H */
