#include "board.h"



board_d* board_create(void)
{	 
	board_d* board = malloc(sizeof(board_d));
	(*board).num_cells_up = 0;
	(*board).num_cells_down = 0;
	
	return board;
}

void board_free(board_d* board)
{
	free(board);
}

char get_cell(board_d* board, int i, int j)
{
	return (*board).cells[i * BOARD_SIZE + j];
}

int get_num_cells_up(board_d* board)
{
	return (*board).num_cells_up;
}

int get_num_cells_down(board_d* board)
{
	return (*board).num_cells_down;
}

/**changes the color of a given board cell without modifying
 * the number of cells the players have.
*/
void set_cell_soft(board_d* board, int i, int j, char color)
{
	(*board).cells[i * BOARD_SIZE + j] = color;
}

/** Changes the color of a given board cell */
void set_cell(board_d* board, int i, int j, char color)
{
	if (get_cell(board, i, j) == 'v')
	{
		(*board).num_cells_down -= 1;
	}
	if (get_cell(board, i, j) == '^')
	{
		(*board).num_cells_up -= 1;
	}
	
	(*board).cells[i * BOARD_SIZE + j] = color;
	
	if (color == 'v')
	{
		(*board).num_cells_down += 1;
	}
	if (color == '^')
	{
		(*board).num_cells_up += 1;
	}
}

/** Prints the current state of the board on screen
 *
 * Implementation note: It would be nicer to do this with ncurse or even
 * SDL/allegro, but this is not really the purpose of this assignment.
 */
void print_board(board_d* board)
{
	printf("\n");
	
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			printf("%c ", get_cell(board, i, j));
		}
		printf("\n");
	}
	printf("\n");
}

/** Creates a random starting board, with a player on two opposite corners */
void rand_board(board_d* board)
{
	srand(time(NULL));
	char rand_color;
	
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			rand_color = 65 + (rand() % 7);
			//memory problems when doing multiple game sets if soft version not used...
			set_cell_soft(board, i, j, rand_color);
		}
	}
	
	//initial state of both players
	set_cell(board, BOARD_SIZE - 1, 0, 'v');
	set_cell(board, 0, BOARD_SIZE - 1, '^');
}

/** Tells if a cell is inside the board */
bool is_inb(int i, int j)
{
	return ((i >= 0 && i < BOARD_SIZE) && (j >= 0 && j < BOARD_SIZE));
}

/** Tells if a cell is next to a given player*/
bool is_next_to_player(board_d* board, char player, int i, int j)
{
	return ((is_inb(i-1, j) && (get_cell(board, i-1, j) == player)) ||
		(is_inb(i+1, j) && (get_cell(board, i+1, j) == player)) ||
		(is_inb(i, j-1) && (get_cell(board, i, j-1) == player)) ||
		(is_inb(i, j+1) && (get_cell(board, i, j+1) == player)));
}

/** NAÏVE FUNCTION:
 * Changes each cell on the board of the given color adjacent
 * to a given player's cell; stops when it accomplishes a full
 * sweep without changing any colors.
*/
void update_board(board_d* board, char player, char color_input)
{
	int i, j;
	bool has_changed = true;

	while (has_changed)
	{
		has_changed = false;

		for (i = 0; i < BOARD_SIZE; i++)
		{
			for (j = 0; j < BOARD_SIZE; j++)
			{
				if (is_next_to_player(board, player, i, j) && (get_cell(board, i, j) == color_input))
				{
					set_cell(board, i, j, player);
					has_changed = true;
				}
			}
		}
	}
}

void update_board_rec(board_d* board, board_d* board_visited, char player, char color_input, int i, int j)
{
	bool is_valid_cell = (get_cell(board,i,j) == color_input || get_cell(board,i,j) == player);
	if (!(get_cell(board_visited,i,j)) && is_valid_cell) 
	{ 
		set_cell(board, i, j, player);
		set_cell(board_visited, i, j, 1);
		
		//calls recursively the adjacent cells to update the board from there
		if (is_inb(i+1,j))
		{
			update_board_rec(board, board_visited, player, color_input, i+1, j);
		}
		if (is_inb (i-1, j))
		{
			update_board_rec(board, board_visited, player, color_input, i-1 ,j);
		}
		if (is_inb (i, j+1))
		{
			update_board_rec(board, board_visited, player, color_input, i, j+1);
		}
		if (is_inb (i, j-1))
		{
			update_board_rec(board, board_visited, player, color_input, i, j-1);
		}
	}
}

void update_board_optimized(board_d* board, char player, char color_input)
{
	int i, j;
	board_d* board_visited = board_create(); 

	//starting coordinates
	if (player == '^')
	{
		i = 0;
		j = BOARD_SIZE - 1;
	}
	else
	{
		i = BOARD_SIZE - 1;
		j = 0;
	}

	//board_visited initialization
	int k, l;
	for (k = 0; k < BOARD_SIZE; k++)
	{
		for (l = 0; l < BOARD_SIZE; l++)
		{
			set_cell(board_visited, k, l, 0);  
		}
	}

	update_board_rec(board, board_visited, player, color_input, i, j);

	board_free(board_visited);
}


