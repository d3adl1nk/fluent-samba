#include "board.h"
#include "IA.h"

/** Calculates the number of new cells gained with a movement by
 * considering the board as a graph and traversing recusively it until
 * all cells are visited, starting at the cell (i, j).
*/
int calc_new_cells_rec(board_d* board, board_d* board_visited, char player, char color, int i, int j)
{
	int num;
	if (get_cell(board, i, j) == color || get_cell(board, i, j) == player)
	{
		if (!(get_cell(board_visited, i ,j)))
		{
			set_cell(board_visited, i, j, 1);
			
			if (get_cell(board, i, j) == color)
			{
				num = 1;
			}
			else if (get_cell(board, i, j) == player)
			{
				num = 0;
			}
			
			//launches a recursive call on each adjacent cell in the board
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
    
    //board_visited initialisation
	int k, l;
	for (k = 0; k < BOARD_SIZE; k++)
    {
		for (l = 0; l < BOARD_SIZE; l++)
		{
			set_cell(board_visited, k, l, 0);  
		}
    }
    
	int num = calc_new_cells_rec(board, board_visited, player, color, i, j);
	
	board_free(board_visited);
	
	return num;
}

/** Maximizes the number of cells gained */
char IA_greedy(board_d* board, char player)
{
	char color, max_color;
	int temp_cells;
	int max_cells = 0;
  
	for (color = 65; color < 72; color++)
	{
		temp_cells = calc_new_cells_optimized(board, player, color);
		if (temp_cells > max_cells)
		{
			max_cells = temp_cells;
			max_color = color;
		}
	}
	return max_color;
}

/** Chooses a random color*/
char IA_random(void)
{
	char rand_color = 65 + (rand() % 7);
	return rand_color;
}

/** Checks which colors are adjacent to a given player's cells */
void available_colors(board_d* board, char player, bool available[7])
{
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			int norm_color = (int) (get_cell(board, i, j) - 65);
			bool is_not_player = (get_cell(board, i, j) != '^') && (get_cell(board, i, j) != 'v');
			
			if (is_not_player && is_next_to_player(board, player, i, j) && (available[norm_color] == false))
			{
				available[norm_color] = true;
			}			
		}
	}
}

/** Chooses a random color among the ones that are adjacent to the IA's cells */
char IA_random_wise(board_d* board, char player)
{
	bool available[7] = { 0 };
	
	available_colors(board, player, available);
		
	char rand_color = 65 + (rand() % 7);
	
	while (available[(int) (rand_color-65)] == false)
	{
		rand_color = 65 + (rand() % 7);
	}
	
	return rand_color;
}

/** Calculates the perimeter of the zone belonging to a given player*/
int border(board_d* board, char player)
{
	int i ,j;
	int num = 0;
	for (i = 0; i< BOARD_SIZE; i++)
	{
		for (j = 0; j< BOARD_SIZE; j++)
		{
			bool is_not_player = (get_cell(board, i, j) != '^') && (get_cell(board, i, j) != 'v');
			if (is_next_to_player(board, player, i, j) && is_not_player)
			{
				num++;
			}
		}
    }
    
	return num;
}

/** Maximises the perimeter of the area contolled by the player */
char IA_greedy_border(board_d* board, char player)
{
	int max_edge_num_cells = 0;
	char color_max;
	int i, j;
	char color = 'A';

	while (color < 72)
	{
		board_d* board_copy = board_create();
		
		for (i = 0; i < BOARD_SIZE; i++)
		{
			for (j = 0; j < BOARD_SIZE; j++)
			{
			  set_cell(board_copy, i, j, (get_cell(board, i,j)));
			}
		}
		
		update_board_optimized(board_copy, player, color);
		
		int edge_num_cells = border(board_copy, player);
		
		if (edge_num_cells > max_edge_num_cells)
		{
			color_max = color;
			max_edge_num_cells = edge_num_cells;
		}
		
		board_free(board_copy);
		
		color ++;
	}
	
	return color_max;
}

/** Same as greedy IA but calculations are based on two turns */
char IA_foresighted_greedy(board_d* board, char player)
{
	char color_max;
	char fst_color;
	
	int max_expansion = 0; 
	
	for (fst_color = 'A'; fst_color < 72; fst_color++)
	{
		board_d* board_copy = board_create();
		
		//copy of the initial state of the board
		int i, j;
		for (i = 0; i < BOARD_SIZE; i++)
		{
			for (j = 0; j < BOARD_SIZE; j++)
			{
				set_cell(board_copy, i, j, (get_cell(board, i,j)));
			}
		}
		
		int fst_expansion = calc_new_cells_optimized(board_copy, player, fst_color);
		int max_scd_expansion = 0;
		
		update_board_optimized(board_copy, player, fst_color);
		
		char scd_color;
		
		//looks for best second color
		for (scd_color = 'A'; scd_color < 72; scd_color++)
		{
			int scd_expansion = calc_new_cells_optimized(board_copy, player, scd_color);
			
			if (scd_expansion > max_scd_expansion)
			{
				max_scd_expansion = scd_expansion;
			}
		}
		
		board_free(board_copy);
		
		//chooses best first color based on total expansion
		if (fst_expansion + max_scd_expansion > max_expansion)
		{
			max_expansion = fst_expansion + max_scd_expansion;
			color_max = fst_color;
		}
	}
	
	return color_max;
}
