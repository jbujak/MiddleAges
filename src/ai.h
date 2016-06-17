/** @file
 *  Interface of AI engine.
 */
#ifndef AI_H
#define AI_H
#include "command.h"
#include "list.h"

/**
 * Initializes AI with position of its king.
 * @param[in] pawn_list List of pawns on board.
 * @param[in] x Initial column of AI king.
 * @param[in] y Initial row of AI king.
 * @param[in] enemy_x Initial column of enemy king.
 * @param[in] enemy_y Initial row of enemy king.
 */
void init_ai(int x, int y, int enemy_x, int enemy_y, struct list *pawn_list);

void clean_ai();

/**
 * Decides what move will AI make.
 * @return Command struct with currently made move.
 */
void get_ai_move(struct command *buf);

void remove_ai_pawn(struct pawn *pawn);

void add_ai_pawn(struct pawn *pawn);

#endif //AI_H
