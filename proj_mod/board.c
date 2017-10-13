#include "board.h"



board_d* board_create(void)
{	
	//char cells[BOARD_SIZE * BOARD_SIZE] = { 0 };
	//board_d init_board = {{ 0 }, 1, 1};
	//board_d* board = &init_board;
	
	board_d* board = malloc(sizeof(board_d));
	(*board).num_cells_up = 1;
	(*board).num_cells_down = 1;
	
	return board;
}

void board_free(board_d* board)
{
	free(board);
}

/** Retrieves the color of a given board cell */
char get_cell(board_d* board, int x, int y)
{
    return (*board).cells[y * BOARD_SIZE + x];
}

/** Changes the color of a given board cell */
void set_cell(board_d* board, int x, int y, char color)
{
    (*board).cells[y * BOARD_SIZE + x] = color;
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
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
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
			set_cell(board, i, j, rand_color);
		}
	}
	
	//initial state of both players
	set_cell(board, BOARD_SIZE - 1, 0, 'v');
	set_cell(board, 0, BOARD_SIZE - 1, '^');
}

/** Tells if a cell is inside the board */
bool is_inb(int x, int y)
{
	return ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE));
}

/** Tells if a cell is next to a given player*/
bool is_next_to_player(board_d* board, char player, int x, int y)
{
	return ((is_inb(x-1, y) && (get_cell(board, x-1, y) == player)) ||
			(is_inb(x+1, y) && (get_cell(board, x+1, y) == player)) ||
			(is_inb(x, y-1) && (get_cell(board, x, y-1) == player)) ||
			(is_inb(x, y+1) && (get_cell(board, x, y+1) == player)));
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
