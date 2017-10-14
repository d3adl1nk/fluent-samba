#include "board.h"



board_d* board_create(void)
{	
	//char cells[BOARD_SIZE * BOARD_SIZE] = { 0 };
	//board_d init_board = {{ 0 }, 1, 1};
	//board_d* board = &init_board;
	
	board_d* board = malloc(sizeof(board_d));
	(*board).num_cells_up = 0;
	(*board).num_cells_down = 0;
	
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

int get_num_cells_up(board_d* board)
{
  return (*board).num_cells_up;
}

int get_num_cells_down(board_d* board)
{
  return (*board).num_cells_down;
}

/** Changes the color of a given board cell */
void set_cell(board_d* board, int x, int y, char color)
{
  if (get_cell(board, x, y) == 'v')
    {
      (*board).num_cells_down -= 1;
    }
  if (get_cell(board, x, y) == '^')
    {
      (*board).num_cells_up -= 1;
    }
  
  (*board).cells[y * BOARD_SIZE + x] = color;
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

void update_board_rec(board_d* board, board_d* board_visited, char player, char color_input, int i, int j)
{
  if (!(get_cell(board_visited,i,j)) && 
      ( get_cell(board,i,j) == color_input ||
	get_cell(board,i,j) == player) ) 
    { 
      set_cell(board, i, j, player);
      set_cell(board_visited, i, j, 1);
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


