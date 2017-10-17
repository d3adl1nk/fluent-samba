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
	//printf("###########################################################\n");
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
	printf("\n\nSTARTING POSITION\n");
	
	//rand_board(board);
	print_board(board);
	//print_occupation(board);
}

void game_loop(board_d* board, char* player, bool is_first_game)
{
	char color_input;
	
  	while (!is_end(board))
	{
		switch (*player)
		{
			case '^':
				(is_first_game == true) ?
				(color_input = IA_greedy(board, *player)) : (color_input = IA_greedy(board, *player));
				break;
	  
			case 'v':
				(is_first_game == true) ?
				(color_input = IA_greedy(board, *player)) : (color_input = IA_greedy(board, *player));
				break;
	  
			default:
				printf("\nERROR\n");
		}			
		update_board_optimized(board, *player, color_input);

		//printf("\nTHE IA HAS CHOSEN THIS COLOR: %c\n", color_input);
		//print_board(board);
		//print_occupation(board);
	  
		*player = change_player(*player);
	}
}

void game_end(board_d* board, char* player)
{
	*player = change_player(*player);
	
	printf("\nEND SITUATION\n");
	print_board(board);
	print_occupation(board);
	//printf("###########################################################\n");
	printf("\nPLAYER %c WON!\n\n", *player);
	printf("###########################################################\n");
}

void board_copy(board_d* dest, board_d* src)
{
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			set_cell(dest, i, j, get_cell(src, i, j));
		}
	}
	
	(*dest).num_cells_up = (*src).num_cells_up;
	(*dest).num_cells_down = (*src).num_cells_down;
}

void game_set(int* up_net_wins)
{
	bool is_first_game = true;
	
	board_d* board_play = board_create();
	board_d* board_save = malloc(sizeof(board_d));

	rand_board(board_play);

	board_copy(board_save, board_play);

	char player[1] = "^";
	
	int i;
	for (i = 0; i < 2; i++)
	{		
		game_init(board_play);
		game_loop(board_play, player, is_first_game);
		game_end(board_play, player);
		
		if (is_first_game)
		{		
			(((*player) == '^') ? (*up_net_wins)++ : (*up_net_wins)--);
		}
		else
		{
			(((*player) == '^') ? (*up_net_wins)-- : (*up_net_wins)++);
		}
		
		if (is_first_game)
		{
			is_first_game = false;
			board_copy(board_play, board_save);
		}
	}
	
	board_free(board_save);
	board_free(board_play);
}

/** Program entry point */
int main(int argc, char *argv[])
{
	int set_num;
	
	if (argc == 2)
	{
		set_num = atoi(argv[1]);
	}
	else if (argc == 1)
	{
		set_num = 100;
	}
	else
	{
		printf("TOO MANY PARAMETERS");
		set_num = -1;
	}
	
	int start_value = 0;
	int* up_net_wins = &start_value;
	
	int i;
	
	for (i = 0; i < set_num; i++)
	{
		game_set(up_net_wins);
		/* sleep lets the seed (local time) used for rand_board
		change so the matches are different each time */
		sleep(1);
	}
	
	printf("\nUP PLAYER NET WINS OVER %d GAME SETS : %d\n", set_num, *up_net_wins);
	
  return 0; // Everything went well
}
