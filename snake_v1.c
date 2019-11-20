#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>

#define init_x 7
#define init_y 5
#define vertical 40
#define horizontal 150

typedef struct food {
	int x;
	int y;
	char symbol;
} food;

typedef struct snake {
	int len;	// length
	int tail_x;
	int tail_y;
	int head_x;
	int head_y;
	//char dir;	// direction
	//char prev;	// previous direction
	char symbol;
} snake;

typedef struct snake_loc {
	int twod_array[vertical-1][horizontal-1];	// maps snake
} snake_loc;

void gotoxy(int, int);
void set_borders();
void init_food(food *food1, snake_loc *loc1);
void print_food(food *food1);
void init_snake(snake *snake1);
void init_map(snake_loc *loc1);
void print_snake(snake *snake1, snake_loc *loc1);

int main() {
	int i;
	int j;
	int k;
	food food1;
	snake snake1;
	snake_loc loc1;

	system("clear");
	system("stty -echo");


	init_map(&loc1);
	init_snake(&snake1);
	set_borders();

	gotoxy(init_x, init_y);		// initial snake location
	print_snake(&snake1, &loc1);

	for (i = 0; i < 5; i++) {
		init_food(&food1, &loc1);
		print_food(&food1);
	}

	//system("/bin/stty raw");

	system("/bin/stty cooked");
	system("stty echo");
	//system("clear");

	gotoxy(0, vertical);
	printf("\n\nBorder set.\n(x by y = 200 by 40 *real: 199 by 39 for absence of 1)\n\n");
	printf("Food initialization and placement tested\n(1 of x & y axis missing)\n");
	printf("(Addressed possible conflict with snake position & missing 1 on axes)\n\n");
	printf("Snake initialized. Initial location set.\n(twod_array displays 1 for snake presence)\n\n");

	for (k = 0; k < vertical - 2; k++) {
		if (k != 1) {
			for (j = 0; j < horizontal - 2; j++) {
				if (j != 1)
					printf("%d", loc1.twod_array[k][j]);
			}
			printf("\n");
		}
	}

	return 0;
}

// set "cursor"
void gotoxy(int x, int y) {
	printf("%c[%d;%df", 0x1B, y, x);
}

void set_borders() {
	int i;

	// print horizontal borders
	for (i = 0; i < horizontal + 1; ++i) {
		// top
		gotoxy(i, 0);
		printf("X");
		// bottom
		gotoxy(i, vertical);
		printf("X");
	}

	// print vertical borders
	for (i = 0; i < vertical + 1; ++i) {
		// left
		gotoxy(0, i);
		printf("H");
		// right
		gotoxy(horizontal, i);
		printf("H");
	}
}

// generate random food position
void init_food(food *food1, snake_loc *loc1) {
	int i = 0;
	while (i == 0) {
		food1->x = (rand() % (horizontal-2)) + 2;
		food1->y = (rand() % (vertical-2)) + 2;
		if ((loc1->twod_array[food1->y][food1->x] == 1) || food1->x == 1 || food1->y == 1)
			i = 0;
		else
			i = 1;
	}
	food1->symbol = '+';
}

// place/print snake food symbol
void print_food(food *food1) {
	gotoxy(food1->x, food1->y);
	printf("%c", food1->symbol);
}

void init_snake(snake *snake1) {
	snake1->len = 10;	// length
	snake1->tail_x = init_x;
	snake1->tail_y = init_y;
	snake1->head_x = (snake1->tail_x) + (snake1->len) - 1;
	snake1->head_y = snake1->tail_y;
	//snake1->dir = right;	// direction
	//snake1->prev = down;	// previous direction
	snake1->symbol = 'o';
}
	
void init_map(snake_loc *loc1) {
	memset(loc1, 0, sizeof(*loc1));
}

void print_snake(snake *snake1, snake_loc *loc1) {
	int i;
	for (i = 0; i < (snake1->len); ++i) {
		printf("%c", snake1->symbol);
		loc1->twod_array[snake1->tail_y][snake1->tail_x] = 1;
		snake1->tail_x += 1;
	}
}


