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

int calc_new_cells_rec(board_d* board, board_d* board_visited, char player, char color, int i, int j)
{
  int num;
  if (get_cell(board, i, j) == color || get_cell(board, i, j) == player)
    {
      if (!(get_cell(board_visited, i ,j)))
	{
	  printf ("(i = %d, j = %d, color = %c)\n", i, j, color); 
	  set_cell(board_visited, i, j, 1);
	  if (get_cell(board, i, j) == color)
	    {
	      num = 1;
	    }
	  else if (get_cell(board, i, j) == player)
	    {
	      num = 0;
	    }
	  if (is_inb(i+1,j))
	    {
	      num += (calc_new_cells_rec(board, board_visited, player, color, i+1, j));
	    }
	  if (is_inb (i-1, j))
	    {
	      num += (calc_new_cells_rec(board, board_visited, player, color, i-1 ,j));
	    }
	  if (is_inb (i, j+1))
	    {
	      num += (calc_new_cells_rec(board, board_visited, player, color, i, j+1));
	    }
	  if (is_inb (i, j-1))
	    {
	      num += (calc_new_cells_rec(board, board_visited, player, color, i, j-1));
	    }
	  return num;
	}
      else
	{
	  return 0;
	}
    }
  return 0;
}

int calc_new_cells_optimized(board_d* board, char player, char color)
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
  int num = calc_new_cells_rec(board, board_visited, player, color, i, j);
  printf("couleur  %c : %d\n", color,  num);
  board_free(board_visited);
  return num;
}


char IA_greedy(board_d* board)
{
	char color, max_color;
	int temp_cells;
	int max_cells = 0;
	
	for (color = 65; color < 72; color++)
	{
		temp_cells = calc_new_cells_optimized(board, 'v', color);
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
