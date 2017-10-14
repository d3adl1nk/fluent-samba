// NOTE: variables for a for loop must be defined before the loop
// NOTE2 : by default, the player is '^' and the IA 'v'

/* TODO:
 * - define constants for player characters?
 * - display color chosen by IA
 * - reduce main function into fragments
*/

#include "board.h"
#include "IA.h"

/* Template of the 7 wonders of the world of the 7 colors assigment. */

/** Represent the actual current board game
 *
 * NOTE: global variables are usually discouraged (plus encapsulation in
 *     an appropriate data structure would also be preferred), but don't worry.
 *     For this first assignment, no dinosaure will get you if you do that.
 */

/*
char board_visited[BOARD_SIZE * BOARD_SIZE] = { 0 }; // Filled with zeros, will be used for the third question !

void update_board_rec(char player, char color_input, int i, int j)
{
  if (!(board_visited[i*BOARD_SIZE + j]) && 
      (get_cell(i,j) == color_input || get_cell(i,j) == player)) 
    { 
      set_cell(i, j, player);
      board_visited[i*BOARD_SIZE + j] = 1;
      if (is_inb(i+1,j))
	{
	  update_board_rec(player, color_input, i+1, j);
	}
      if (is_inb (i-1, j))
	{
	  update_board_rec(player, color_input, i-1 ,j);
	}
      if (is_inb (i, j+1))
	{
	  update_board_rec(player, color_input, i, j+1);
	}
      if (is_inb (i, j-1))
	{
	  update_board_rec(player, color_input, i, j-1);
	}
    }
}

void update_board_optimized(char player, char color_input)
{
  int i, j;
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
	  board_visited[k*BOARD_SIZE + l] = 0;  
	}
    }
  update_board_rec(player, color_input, i, j);
}
*/


/** TEST FUNCTION: iterates sthg on every cell of the board */
/*
void test_on_board(void)
{
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
		  printf("%d ", get_cell(board, i, j));			
		}
		printf("\n");
	}
	printf("\n");
}
*/

/** Asks the player for a valid color input */
char player_input(void)
{
	char color_input;
	printf("\nPLEASE ENTER A COLOR: ");
	scanf(" %c", &color_input);
	
	while ((color_input > 71) || (color_input < 65))
	{
		printf("PLEASE ENTER A VALID COLOR: ");
		scanf(" %c", &color_input);
	}
	
	printf("\n");
	
	return color_input;
}

/* Updates the number of cells each player owns */
/* void update_num_cells(board_d* board, char player) */
/* { */
/* 	if (player == '^') */
/* 	{ */
/* 		(*board).num_cells_up = 0; */
/* 	} */
/* 	else if (player == 'v') */
/* 	{ */
/* 		(*board).num_cells_down = 0; */
/* 	} */
	
/* 	int i, j; */
/* 	for (i = 0; i < BOARD_SIZE; i++) */
/* 	{ */
/* 		for (j = 0; j < BOARD_SIZE; j++) */
/* 		{ */
/* 			if ((player == '^') && (get_cell(board, i, j) == '^')) */
/* 			{ */
/* 				(*board).num_cells_up++; */
/* 			} */
/* 			else if ((player == 'v') && (get_cell(board, i, j) == 'v')) */
/* 			{ */
/* 				(*board).num_cells_down++; */
/* 			} */
/* 		} */
/* 	} */
/* } */

/** Prints the percentage of the board each player owns */
void print_occupation(board_d* board)
{
  int perc_up = 100 * get_num_cells_up(board) / (BOARD_SIZE*BOARD_SIZE);
  int perc_down = 100 * get_num_cells_down(board) / (BOARD_SIZE*BOARD_SIZE);

	printf("PLAYER ^: %d\nPLAYER v: %d\n", perc_up, perc_down);
	printf("###########################################################\n");
}

/** Checks if the victory condition is met */
bool is_end(board_d* board)
{	
  int perc_up = 100 * get_num_cells_up(board) / (BOARD_SIZE*BOARD_SIZE);
  int perc_down = 100 * get_num_cells_down(board) / (BOARD_SIZE*BOARD_SIZE);
	
	return ((perc_up > 50) || (perc_down > 50));
}

/**Given a player, returns the other one*/
char change_player(char curr_player)
{
	return (curr_player == '^') ? 'v' : '^';
}

/** Program entry point */
int main(void)
{
    printf("\n\nWelcome to the 7 wonders of the world of the 7 colors\n"
	   "*****************************************************\n\n"
	   "Current board state:\n");
    
    char color_input;
    char player = '^';
    
    board_d* board;
    board = board_create();
	
	bool is_IA_turn = false;
	
	rand_board(board);
    print_board(board);
    print_occupation(board);
    
	while (!is_end(board))
	{
		switch (player)
		{
			case '^':
				color_input = player_input();
				is_IA_turn = false;
				break;
			
			case 'v':
				color_input = IA_greedy(board);
				is_IA_turn = true;
				break;
				
			default:
				printf("\nERROR\n");
		}			
		
		update_board_optimized(board, player, color_input);
		
		if (is_IA_turn)
		{
			printf("\nTHE IA HAS CHOSEN THIS COLOR: %c\n", color_input);
			print_board(board);
		}
		
		/* update_num_cells(board, player); */
		printf("occupation:");
		print_occupation(board);
		
		player = change_player(player);
	}
	
	print_board(board);
	
	//free(board);

    return 0; // Everything went well
}
