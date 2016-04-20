#include <stdio.h>
#include <stdlib.h>

void start_game() {

}

void end_game() {

}

void print_topleft() {

}

int init(int n, int k, int p, int x1, int y1, int x2, int y2) { 
	printf("INIT\n\tn:%d\n\tk:%d\n\tp:%d\n\tx1:%d\n\ty1:%d\n\tx2:%d\n\ty2:%d\n\n",
		n, k, p, x1, y1, x2, y2);

	return 0;
}

int move(int x1, int y1, int x2, int y2) {
	printf("MOVE\n\tx1:%d\n\ty1:%d\n\tx2:%d\n\ty2:%d\n\n",
		x1, y1, x2, y2);
	return 0;
}

int produce_knight(int x1, int y1, int x2, int y2) {
	printf("PRODUCE_KNIGTH\n\tx1:%d\n\ty1:%d\n\tx2:%d\n\ty2:%d\n\n",
		x1, y1, x2, y2);
	return 0;
}

int produce_peasant(int x1, int y1, int x2, int y2) {
	printf("PRODUCE_PEASANT\n\tx1:%d\n\ty1:%d\n\tx2:%d\n\ty2:%d\n\n",
		x1, y1, x2, y2);
	return 0;
}

int end_turn() {
	printf("END_TURN\n\n");
	return 0;
}
