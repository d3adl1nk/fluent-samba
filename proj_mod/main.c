// NOTE : by default, the player is '^' and the IA 'v'

#include "board.h"
#include "IA.h"

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

/** Prints the percentage of the board each player owns */
void print_occupation(board_d* board)
{
	int perc_up = 100 * get_num_cells_up(board) / (BOARD_SIZE*BOARD_SIZE);
	int perc_down = 100 * get_num_cells_down(board) / (BOARD_SIZE*BOARD_SIZE);

	printf("OCCUPATION:\nPLAYER ^: %d\nPLAYER v: %d\n", perc_up, perc_down);
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

void game_init(board_d* board)
{
	printf("\n\nWelcome to the 7 wonders of the world of the 7 colors\n"
			"*****************************************************\n\n"
			"Current board state:\n");
	
	rand_board(board);
	print_board(board);
	print_occupation(board);
}

void game_loop(board_d* board, char* player, bool* is_IA_turn)
{
	char color_input;
	
  	while (!is_end(board))
	{
		switch (*player)
		{
			case '^':
				color_input = player_input();
				*is_IA_turn = false;
				break;
	  
			case 'v':
				color_input = IA_random_wise(board, *player);
				//color_input = player_input();
				*is_IA_turn = true;
				break;
	  
			default:
				printf("\nERROR\n");
		}			
		update_board_optimized(board, *player, color_input);
	  
		if (*is_IA_turn)
		{
			printf("\nTHE IA HAS CHOSEN THIS COLOR: %c\n", color_input);
			print_board(board);
		}
	  
		print_occupation(board);
	  
		*player = change_player(*player);
	}
}

void game_end(board_d* board, char* player)
{
	*player = change_player(*player);
	
	print_board(board);
	printf("###########################################################\n");
	printf("\nPLAYER %c WON!\n\n", *player);
	board_free(board);
}

/** Program entry point */
int main(void)
{
	board_d* board = board_create();
	char player[1] = "^";
	
	bool does_IA_start = false;
	bool* is_IA_turn = &does_IA_start;
	
	game_init(board);
	game_loop(board, player, is_IA_turn);
	game_end(board, player);
  
  return 0; // Everything went well
}
