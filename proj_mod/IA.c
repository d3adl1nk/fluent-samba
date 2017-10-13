#include "board.h"
#include "IA.h"

/** Calculates cells gained by a player with a certain move */
//TODO: Needs to change -> same model as update_board
int calc_new_cells(board_d* board, char player, char color)
{
	int num_new_cells = 0;
	
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (is_next_to_player(board, player, i, j) && get_cell(board, i, j) == color)
			{
				num_new_cells++;
			}
		}
	}
	
	return num_new_cells;
}

char IA_greedy(board_d* board)
{
	char color, max_color;
	int temp_cells;
	int max_cells = 0;
	
	for (color = 65; color < 72; color++)
	{
		temp_cells = calc_new_cells(board, 'v', color);
		if (temp_cells > max_cells)
		{
			max_cells = temp_cells;
			max_color = color;
		}
	}
	
	return max_color;
}

/** The IA chooses a random color*/
char IA_random(void)
{
	char rand_color = 65 + (rand() % 7);
	return rand_color;
}
