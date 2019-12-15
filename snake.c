#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include </usr/include/python2.7/Python.h>
#include <pthread.h>
#include <time.h>

#define init_x 7
#define init_y 9
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
	char symbol;
} snake;

typedef struct snake_loc {
	int twod_array[vertical][horizontal];	// maps snake
} snake_loc;

int mouse_direction();
void gotoxy(int x, int y);
void set_borders();
void init_food(food *food1, snake_loc *loc1);
void print_food(food *food1);
void init_snake(snake *snake1);
void init_map(snake_loc *loc1);
void print_snake(snake *snake1, snake_loc *loc1);
void print_snake2(snake *snake1, snake_loc *loc1);
int snake_move(snake *snake1, snake_loc *loc1, food *food1, int score);
int game_over(snake *snake1, snake_loc *loc1);
void *get_dir();

int dir = 3;	// right

int main() {
	int score = 0;
	int end = 0;
	food food1;
	snake snake1;
	snake_loc loc1;
	pthread_t tid;
	struct timespec tim,tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = 50000000L;

	system("clear");

	init_map(&loc1);		// zero out map
	init_snake(&snake1);
	set_borders();
	gotoxy(init_x, init_y);		// initial snake location
	print_snake(&snake1, &loc1);	// print snake in map
	print_snake2(&snake1, &loc1);	// print snake in game
	init_food(&food1, &loc1);
	print_food(&food1);

	pthread_create(&tid, NULL, get_dir, NULL);	// thread for obtaining mouse direction

	while(!end){
		gotoxy(0, vertical + 6);
		printf("Score = %d\n", score);		// updates score real-time
		if(dir == 3 || dir == 4){
			tim.tv_sec = 0;
			tim.tv_nsec = 50000000L;
		}
		else{
			tim.tv_sec = 0;
			tim.tv_nsec = 100000000L;
		}
		nanosleep(&tim,&tim2);
		end = game_over(&snake1, &loc1);
		score = snake_move(&snake1, &loc1, &food1, score);
	}

	gotoxy(0, vertical + 6);
	printf("Game Over! Score = %d\n", score);

	return 0;
}

// set "cursor"
void gotoxy(int x, int y) {
	printf("%c[%d;%df", 0x1B, y, x);
}

void set_borders() {
	int i;

	// print horizontal borders
	for (i = 1; i < horizontal; ++i) {
		// top
		gotoxy(i, 0);
		printf("X");
		// bottom
		gotoxy(i, vertical);
		printf("X");
	}

	// print vertical borders
	for (i = 1; i < vertical; ++i) {
		// left
		gotoxy(0, i);
		printf("H");
		// right
		gotoxy(horizontal, i);
		printf("H");
	}
	gotoxy(0, 0);
	printf("Y");
	gotoxy(horizontal, 0);
	printf("Y");
	gotoxy(0, vertical);
	printf("Y");
	gotoxy(horizontal, vertical);
	printf("Y");
}

// generate random food position
void init_food(food *food1, snake_loc *loc1) {
	int i = 0;
	while (i == 0) {
		food1->x = (rand() % (horizontal-2)) + 2;
		food1->y = (rand() % (vertical-2)) + 2;
		if ((loc1->twod_array[food1->y][food1->x] > 0) || food1->x == 1 || food1->y == 1)
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
	snake1->symbol = 'o';
}
	
void init_map(snake_loc *loc1) {
	memset(loc1, 0, sizeof(*loc1));
}

void print_snake(snake *snake1, snake_loc *loc1) {
	int i;
	for (i = 0; i < (snake1->len); ++i) {
		loc1->twod_array[snake1->head_y][snake1->head_x-i] = i+1;
	}
}

void print_snake2(snake *snake1, snake_loc *loc1){
	int i;
	int j;

	for(i = 2; i < vertical; i++){
		for(j = 2; j < horizontal; j++){
			if(loc1->twod_array[i][j] > 0){
				gotoxy(j,i);
				printf("%c", snake1->symbol);
			}
		}
	}
}

int snake_move(snake *snake1, snake_loc *loc1, food *food1, int score){
	int i;
	int j;
	int temp_x = 0;
	int temp_y = 0;

	switch(dir){//move_head
		case 1: snake1->head_y--; break;
		case 2: snake1->head_y++; break;
		case 3: snake1->head_x++; break;
		case 4: snake1->head_x--; break;
	}

	for(i = 0; i < vertical; i++){
		for(j = 0; j < horizontal; j++){
			if(loc1->twod_array[i][j] == snake1->len){
				temp_y = i;
				temp_x = j;
				loc1->twod_array[i][j] = 0;
				gotoxy(j,i);
				printf(" ");
			}
			if(loc1->twod_array[i][j] > 0){
				loc1->twod_array[i][j]++;
				gotoxy(j,i);
				printf(" ");
			}
			if(loc1->twod_array[i][j] == snake1->len){
				snake1->tail_y = i;
				snake1->tail_x = j;
			}
		}
	}
	loc1->twod_array[snake1->head_y][snake1->head_x] = 1;

	if(snake1->head_x == food1->x && snake1->head_y == food1->y){		// food eaten
		snake1->tail_y = temp_y;					// longer snake
		snake1->tail_x = temp_x;
		snake1->len++;
		loc1->twod_array[snake1->tail_y][snake1->tail_x] = snake1->len;
		score++;
		init_food(food1, loc1);
		print_food(food1);
	}

	print_snake2(snake1, loc1);

	return score;
}

int game_over(snake *snake1, snake_loc *loc1){
	int collision_x = snake1->head_x;
	int collision_y = snake1->head_y;
	
	switch(dir){//move_head
		case 1: collision_y--; break;
		case 2: collision_y++; break;
		case 3: collision_x++; break;
		case 4: collision_x--; break;
	}
	if ((loc1->twod_array[collision_y][collision_x] > 0) || collision_x < 2 || collision_y < 2 || collision_x == horizontal || collision_y == vertical){
		return 1;
	}
	return 0;
}

int mouse_direction(){
	PyObject *pName, *pModule, *pFunc;
	PyObject *syspath, *pValue;
	char *eximpath = "/home/veaubuntu/Desktop";
	int value = 0;

	Py_Initialize();

	syspath = PySys_GetObject("path");
	pName = PyString_FromString(eximpath);
	PyList_Insert(syspath,0,pName);
	PySys_SetObject("path",syspath);

	pName = PyString_FromString("project");
	/* Error checking of pName left out */

	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != NULL) {
		PyObject *pythonArgument;
		pythonArgument = PyTuple_New(1);
		pValue = PyString_FromString("dir");

		if (pValue == NULL) {
		     return 1;
		}

		PyTuple_SetItem(pythonArgument, 0, pValue);
		pFunc = PyObject_GetAttrString(pModule, "project");

		if(pFunc && PyCallable_Check(pFunc)){
			pValue = PyObject_CallObject(pFunc, pythonArgument);
			
			if(pValue != NULL){
				value = _PyInt_AsInt(pValue);
			}
			else PyErr_Print();
		}
        }

	else{
		PyErr_Print();
		fprintf(stderr, "Failed to load \"project\"\n");
		return 1;
	}

	return value;
}

void *get_dir(){
	int prev_dir = 0;
	int value = 0;
	while(1){
		prev_dir = dir;
		value = mouse_direction();
		switch(prev_dir){
			case 1: if(value == 2) dir = 1; else dir = value; break;
			case 2: if(value == 1) dir = 2; else dir = value; break;
			case 3: if(value == 4) dir = 3; else dir = value; break;
			case 4: if(value == 3) dir = 4; else dir = value; break;
		}
	}
}
