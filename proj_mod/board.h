#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* rand */
#include <time.h>
#include <stdbool.h>	/* boolean values */

/* We want a 30x30 board game by default */
#define BOARD_SIZE 30

struct board_data
{
	char cells[BOARD_SIZE * BOARD_SIZE];
	int num_cells_up;
	int num_cells_down;
};

typedef struct board_data board_d;

board_d* board_create(void);

void board_free(board_d* board);

char get_cell(board_d* board, int x, int y);
void set_cell(board_d* board, int x, int y, char color);

void print_board(board_d* board);

void rand_board(board_d* board);

void update_board(board_d* board, char player, char color_input);

bool is_inb(int x, int y);
bool is_next_to_player(board_d* board, char player, int x, int y);

#endif /* BOARD_H */
