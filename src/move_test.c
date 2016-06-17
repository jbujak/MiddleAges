/*
 * Unit tests for moving pawn in middle_ages.
 */
#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdbool.h>
#include <string.h>
#include "cmocka.h"

#include "command.h"
#include "engine.h"
#include "list.h"
#include "pawn.h"
#include "parse.h"

#define OK 0
#define ERROR 42

/* Variables used to mock program input */
static int sent_lines;
static int lines_to_send;
static char *lines[1000];

/* Program main function */
extern int middle_ages_main();

/* Declaration of private function to test */
extern int get_number(int n, const char *line, int *error);

/* Program variables used to control program state */
extern struct list *list;
extern int board_size;
extern int init_counter;
extern enum player current_player;

/* Function preparing test environment for move engine test */
static void move_engine_setup();

/* Function cleaning test environment after move engine test */
static void move_engine_teardown();


/* Mock function used to control what input gets tested program on stding */
int mock_fgets(char *line, int n, FILE* const file) {
	if(n || file) {} /* To avoid unesed variable warning */
	if(sent_lines < lines_to_send) {
		strcpy(line, lines[sent_lines]);
		sent_lines++;
	} else {
		strcpy(line, "END_TURN\n");
	}

	return 0;
}

/* Mock function ignoring program output -- we do not need to test it in this task */
int mock_printf() {
	return 0;
}

/* Ensure program exits with valid code if is given correct move */
static void test_move_parse_correct_arguments() {

	lines[0] = "INIT 10 10 1 1 1 1 9\n";
	lines[1] = "MOVE 1 9 1 8\n";

	lines_to_send = 2;
	sent_lines = 0;

	assert_int_not_equal(middle_ages_main(), ERROR);
}

/* Ensure program returns error if move was given to few arguments */
static void test_move_parse_too_few_arguments() {

	lines[0] = "INIT 10 100 1 1 1 1 9\n";
	lines[1] = "MOVE 1 9 1\n";

	lines_to_send = 2;
	sent_lines = 0;

	assert_int_equal(middle_ages_main(), ERROR);
}

/* Ensure program returns error if move was given to many arguments */
static void test_move_parse_too_many_arguments() {

	lines[0] = "INIT 10 100 1 1 1 1 9\n";
	lines[1] = "MOVE 1 9 1 8 5\n";

	lines_to_send = 2;
	sent_lines = 0;

	assert_int_equal(middle_ages_main(), ERROR);
}

/* Ensure program returns error if move was given a non-numeric argument */
static void test_move_parse_non_numeric_arguments() {

	lines[0] = "INIT 10 100 1 1 1 1 9\n";
	lines[1] = "MOVE 1 9 1 a\n";

	lines_to_send = 2;
	sent_lines = 0;

	assert_int_equal(middle_ages_main(), ERROR);
}

/* Ensure program returns error if space is at the end of move command */
static void test_move_parse_space_on_end() {

	lines[0] = "INIT 10 100 1 1 1 1 9\n";
	lines[1] = "MOVE 1 9 1 8 \n";

	lines_to_send = 2;
	sent_lines = 0;

	assert_int_equal(middle_ages_main(), ERROR);
}

/* Ensure program returns error if double space is 
 * between two arguments in move command */
static void test_move_parse_double_space() {

	lines[0] = "INIT 10 100 1 1 1 1 9\n";
	lines[1] = "MOVE 1 9 1  8\n";

	lines_to_send = 2;
	sent_lines = 0;

	assert_int_equal(middle_ages_main(), ERROR);
}

/* Ensure program ends correctly and returns error if move command is too long */
static void test_move_parse_long_line() {

	lines[0] = "INIT 10 100 1 1 1 1 9\n";
	lines[1] = "MOVE 1 9 1 888888888888888888888888888888888888888888888888888888888888888888888888888888\n";

	lines_to_send = 2;
	sent_lines = 0;

	assert_int_equal(middle_ages_main(), ERROR);
}

/* Ensure program does not return error if was given valid move command */
static void test_game_correct_move() {

	lines[0] = "INIT 10 100 1 1 1 1 9\n";
	lines[1] = "MOVE 1 9 1 8\n";

	lines_to_send = 2;
	sent_lines = 0;

	assert_int_not_equal(middle_ages_main(), ERROR);
}

/* Ensure proram returns error if player tries to move his pawn on field
 * occupated by his other pawn */
static void test_game_move_on_own_field() {

	lines[0] = "INIT 10 100 1 1 1 1 9\n";
	lines[1] = "MOVE 1 9 2 9\n";

	lines_to_send = 2;
	sent_lines = 0;

	assert_int_equal(middle_ages_main(), ERROR);
}

/* Ensure program returns error if player tries to move his pawn on the same field
 * this pawn is now */
static void test_game_move_on_same_field() {

	lines[0] = "INIT 10 100 1 1 1 1 9\n";
	lines[1] = "MOVE 1 9 1 9\n";

	lines_to_send = 2;
	sent_lines = 0;

	assert_int_equal(middle_ages_main(), ERROR);
}

/* Ensure program returns error if player tries to move his pawn on too distant field */
static void test_game_move_too_long() {

	lines[0] = "INIT 10 100 1 1 1 1 9\n";
	lines[1] = "MOVE 1 9 1 7\n";

	lines_to_send = 2;
	sent_lines = 0;

	assert_int_equal(middle_ages_main(), ERROR);


	lines[0] = "INIT 10 100 1 1 1 1 9\n";
	lines[1] = "MOVE 1 9 3 9\n";

	lines_to_send = 2;
	sent_lines = 0;

	assert_int_equal(middle_ages_main(), ERROR);

}

/* Ensure program returns error if player tries to move his pawn out of the board */
static void test_game_move_out_of_board() {

	lines[0] = "INIT 10 100 1 1 1 1 9\n";
	lines[1] = "MOVE 1 9 0 9\n";

	lines_to_send = 2;
	sent_lines = 0;

	assert_int_equal(middle_ages_main(), ERROR);
}

/* Ensure program returns error if player tries to move his pawn twice in one turn */
static void test_game_double_move() {

	lines[0] = "INIT 10 100 1 1 1 1 9\n";
	lines[1] = "MOVE 1 9 1 8\n";
	lines[2] = "MOVE 1 8 1 7\n";

	lines_to_send = 3;
	sent_lines = 0;

	assert_int_equal(middle_ages_main(), ERROR);
}

/* Ensure program returns error if player tries to move non-existent pawn */
static void test_game_move_from_empty_field() {


	lines[0] = "INIT 10 100 1 1 1 1 9\n";
	lines[1] = "MOVE 5 5 5 4\n";

	lines_to_send = 2;
	sent_lines = 0;

	assert_int_equal(middle_ages_main(), ERROR);
}

/* Ensure program returns error if player tries to move opponent pawn */
static void test_game_move_opponent_pawn() {

	lines[0] = "INIT 10 100 1 1 1 1 9\n";
	lines[1] = "MOVE 1 1 2 2\n";

	lines_to_send = 2;
	sent_lines = 0;

	assert_int_equal(middle_ages_main(), ERROR);
}

/* Ensure move function does not return error if player move is correct */
static void test_move_engine_correct_move() {
	move_engine_setup();
	current_player = PLAYER_1;

	struct pawn *pawn = create_pawn(4, 5, PEASANT, PLAYER_1, 0);
	add_pawn(pawn, list);

	assert_int_equal(move(4, 5, 5, 5), OK);

	move_engine_teardown();
}

/* Ensure move function returns error if player tries to move on the field
 * occupated by his own pawn */
static void test_move_engine_move_on_own_field() {
	move_engine_setup();
	current_player = PLAYER_1;

	struct pawn *pawn_1 = create_pawn(4, 5, PEASANT, PLAYER_1, 0);
	struct pawn *pawn_2 = create_pawn(5, 5, PEASANT, PLAYER_1, 0);
	add_pawn(pawn_1, list);
	add_pawn(pawn_2, list);

	assert_int_not_equal(move(4, 5, 5, 5), OK);

	move_engine_teardown();
}

/* Ensure move function returns error if player tries to move on the same field */
static void test_move_engine_move_on_the_same_field() {
	move_engine_setup();
	current_player = PLAYER_1;

	struct pawn *pawn = create_pawn(4, 5, PEASANT, PLAYER_1, 0);
	add_pawn(pawn, list);

	assert_int_not_equal(move(4, 5, 4, 5), OK);

	move_engine_teardown();
}

/* Ensure move function returns error if player tries to move too far */
static void test_move_engine_move_too_long() {
	move_engine_setup();
	current_player = PLAYER_1;

	struct pawn *pawn = create_pawn(4, 5, PEASANT, PLAYER_1, 0);
	add_pawn(pawn, list);

	assert_int_not_equal(move(4, 5, 6, 5), OK);

	move_engine_teardown();
}

/* Ensure move function returns error if player tries to move out of board */
static void test_move_engine_move_out_of_board() {
	move_engine_setup();
	current_player = PLAYER_1;

	struct pawn *pawn = create_pawn(1, 1, PEASANT, PLAYER_1, 0);
	add_pawn(pawn, list);

	assert_int_not_equal(move(1, 1, 1, 0), OK);

	move_engine_teardown();
}

/* Ensure move function returns error if player tries to move one pawn twice */
static void test_move_engine_double_move() {
	move_engine_setup();
	current_player = PLAYER_1;

	struct pawn *pawn = create_pawn(4, 5, PEASANT, PLAYER_1, 0);
	add_pawn(pawn, list);

	assert_int_equal(move(4, 5, 5, 5), OK);
	assert_int_not_equal(move(5, 5, 5, 6), OK);

	move_engine_teardown();
}

/* Ensure move function returns error if player tries to move pawn that does not exist */
static void test_move_engine_move_from_empty_field() {
	move_engine_setup();
	current_player = PLAYER_1;

	assert_int_not_equal(move(4, 5, 4, 6), OK);

	move_engine_teardown();
}

/* Ensure move function returns error if player tries to move opponent pawn */
static void test_move_engine_move_opponent_pawn() {
	move_engine_setup();
	current_player = PLAYER_1;

	struct pawn *pawn = create_pawn(4, 5, PEASANT, PLAYER_2, 0);
	add_pawn(pawn, list);

	assert_int_not_equal(move(4, 5, 5, 5), OK);

	move_engine_teardown();
}



/* Ensure program correctly detects fight if player moves his pawn on field
 * occupated by stronger opponent pawn; in result his pawn should be deleted
 * and opponent pawn should remain on its place */
static void test_fight_opponent_stronger() {
	move_engine_setup();
	current_player = PLAYER_1;

	struct pawn *pawn_1 = create_pawn(4, 5, PEASANT, PLAYER_1, 0);
	struct pawn *pawn_2 = create_pawn(5, 5, KNIGHT, PLAYER_2, 0);

	add_pawn(pawn_1, list);
	add_pawn(pawn_2, list);

	assert_int_equal(move(4, 5, 5, 5), OK);

	struct pawn *alive_pawn = get_pawn(5, 5, list);

	assert_null(get_pawn(4, 5, list));
	assert_non_null(alive_pawn);

	assert_int_equal(alive_pawn->type, KNIGHT);
	assert_int_equal(alive_pawn->player, PLAYER_2);

	move_engine_teardown();
}

/* Ensure program correctly detects fight if player moves his pawn on field
 * occupated by weaker opponent pawn; in result his pawn should be moved
 * and opponent pawn should be destroyed */
static void test_fight_opponent_weaker() {
	move_engine_setup();
	current_player = PLAYER_1;

	struct pawn *pawn_1 = create_pawn(4, 5, KNIGHT, PLAYER_1, 0);
	struct pawn *pawn_2 = create_pawn(5, 5, PEASANT, PLAYER_2, 0);

	add_pawn(pawn_1, list);
	add_pawn(pawn_2, list);

	assert_int_equal(move(4, 5, 5, 5), OK);

	struct pawn *alive_pawn = get_pawn(5, 5, list);

	assert_null(get_pawn(4, 5, list));
	assert_non_null(alive_pawn);

	assert_int_equal(alive_pawn->type, KNIGHT);
	assert_int_equal(alive_pawn->player, PLAYER_1);

	move_engine_teardown();
}

/* Ensure program correctly detects fight if player moves his pawn on field
 * occupated by equal opponent pawn; in result both pawns should be deleted */
static void test_fight_opponent_equal() {
	move_engine_setup();
	current_player = PLAYER_1;

	struct pawn *pawn_1 = create_pawn(4, 5, PEASANT, PLAYER_1, 0);
	struct pawn *pawn_2 = create_pawn(5, 5, PEASANT, PLAYER_2, 0);

	add_pawn(pawn_1, list);
	add_pawn(pawn_2, list);

	assert_int_equal(move(4, 5, 5, 5), OK);

	assert_null(get_pawn(4, 5, list));
	assert_null(get_pawn(5, 5, list));

	move_engine_teardown();
}

/* Tests of a get_number function */

/* Ensure get_number returns correct values when given a correct number */
static void test_get_number_correct_values() {
	int error = OK;

	assert_int_equal(get_number(0, "123 abc 0 def 2147483647\n", &error), 123);
	assert_int_equal(error, OK);
	assert_int_equal(get_number(2, "123 abc 0 def 2147483647\n", &error), 0);
	assert_int_equal(error, OK);
	assert_int_equal(get_number(4, "123 abc 0 def 2147483647\n", &error), 2147483647);
	assert_int_equal(error, OK);
}

/* Ensure get_numer returns error when given a non-numeric value */
static void test_get_number_non_numeric_value() {
	int error = OK;
	get_number(0, "x\n", &error);
	assert_int_not_equal(error, OK);

	error = OK;
	get_number(0, "\n", &error);
	assert_int_not_equal(error, OK);

	error = OK;
	get_number(0, "123x\n", &error);
	assert_int_not_equal(error, OK);

	error = OK;
	get_number(0, "x123\n", &error);
	assert_int_not_equal(error, OK);
}

/* Ensure get_number returns error when word at given position does not exist */
static void test_get_number_word_out_of_line() {
	int error = OK;
	get_number(1, "123\n", &error);
	assert_int_not_equal(error, OK);

	error = OK;
	get_number(-1, "123\n", &error);
	assert_int_not_equal(error, OK);
}

static int test_setup() {
	return 0;
}

static int test_teardown() {
	return 0;
}

static void move_engine_setup() {
	list = create_list();
	board_size = 10;
	init_counter = 1;
}

static void move_engine_teardown() {
	free_list(list);
}

int main() {
	const struct CMUnitTest move_tests[] = {
		cmocka_unit_test(test_move_parse_correct_arguments),
		cmocka_unit_test(test_move_parse_too_few_arguments),
		cmocka_unit_test(test_move_parse_too_many_arguments),
		cmocka_unit_test(test_move_parse_non_numeric_arguments),
		cmocka_unit_test(test_move_parse_space_on_end),
		cmocka_unit_test(test_move_parse_double_space),
		cmocka_unit_test(test_move_parse_long_line),

		cmocka_unit_test(test_move_engine_correct_move),
		cmocka_unit_test(test_move_engine_move_on_own_field),
		cmocka_unit_test(test_move_engine_move_on_the_same_field),
		cmocka_unit_test(test_move_engine_move_too_long),
		cmocka_unit_test(test_move_engine_move_out_of_board),
		cmocka_unit_test(test_move_engine_double_move),
		cmocka_unit_test(test_move_engine_move_from_empty_field),
		cmocka_unit_test(test_move_engine_move_opponent_pawn),

		cmocka_unit_test(test_fight_opponent_stronger),
		cmocka_unit_test(test_fight_opponent_weaker),
		cmocka_unit_test(test_fight_opponent_equal),

		cmocka_unit_test(test_game_correct_move),
		cmocka_unit_test(test_game_move_on_own_field),
		cmocka_unit_test(test_game_move_on_same_field),
		cmocka_unit_test(test_game_move_too_long),
		cmocka_unit_test(test_game_move_out_of_board),
		cmocka_unit_test(test_game_double_move),
		cmocka_unit_test(test_game_move_from_empty_field),
		cmocka_unit_test(test_game_move_opponent_pawn),

		cmocka_unit_test(test_get_number_correct_values),
		cmocka_unit_test(test_get_number_non_numeric_value),
		cmocka_unit_test(test_get_number_word_out_of_line),
	};

	return cmocka_run_group_tests(move_tests, test_setup, test_teardown);
}
