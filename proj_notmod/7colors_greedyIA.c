// NOTE: variables for a for loop must be defined before the loop
// NOTE2 : by default, the player is '^' and the IA 'v'

/* TODO:
 * - define constants for player characters?
 * - display color chosen by IA
 * - reduce main function into fragments
*/


/* Template of the 7 wonders of the world of the 7 colors assigment. */

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* rand */
#include <time.h>
#include <stdbool.h>	/* boolean values */

/* We want a 30x30 board game by default */
#define BOARD_SIZE 30


/** Represent the actual current board game
 *
 * NOTE: global variables are usually discouraged (plus encapsulation in
 *     an appropriate data structure would also be preferred), but don't worry.
 *     For this first assignment, no dinosaure will get you if you do that.
 */
char board[BOARD_SIZE * BOARD_SIZE] = { 0 }; // Filled with zeros

/** Retrieves the color of a given board cell */
char get_cell(int x, int y)
{
    return board[y * BOARD_SIZE + x];
}

/** Changes the color of a given board cell */
void set_cell(int x, int y, char color)
{
    board[y * BOARD_SIZE + x] = color;
}

/** Prints the current state of the board on screen
 *
 * Implementation note: It would be nicer to do this with ncurse or even
 * SDL/allegro, but this is not really the purpose of this assignment.
 */
void print_board(void)
{
	printf("\n");
	
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", get_cell(i, j));
        }
        printf("\n");
    }
    printf("\n");
}

/** Creates a random starting board, with a player on two opposite corners */
void rand_board(void)
{
	srand(time(NULL));
	char rand_color;
	
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			rand_color = 65 + (rand() % 7);
			set_cell(i, j, rand_color);
		}
	}
	
	//initial state of both players
	set_cell(BOARD_SIZE - 1, 0, 'v');
	set_cell(0, BOARD_SIZE - 1, '^');
}

/** Tells if a cell is inside the board */
bool is_inb(int x, int y)
{
	return ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE));
}

/** Tells if a cell is next to a given player*/
bool is_next_to_player(char player, int x, int y)
{
	return ((is_inb(x-1, y) && (get_cell(x-1, y) == player)) ||
			(is_inb(x+1, y) && (get_cell(x+1, y) == player)) ||
			(is_inb(x, y-1) && (get_cell(x, y-1) == player)) ||
			(is_inb(x, y+1) && (get_cell(x, y+1) == player)));
}

/** NAÏVE FUNCTION:
 * Changes each cell on the board of the given color adjacent
 * to a given player's cell; stops when it accomplishes a full
 * sweep without changing any colors.
*/
void update_board(char player, char color_input)
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
				if (is_next_to_player(player, i, j) && (get_cell(i, j) == color_input))
				{
					set_cell(i, j, player);
					has_changed = true;
				}
			}
		}
	}
}

/** TEST FUNCTION: iterates sthg on every cell of the board */
void test_on_board(char player)
{
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			printf("%d ", is_next_to_player(player, i, j));			
		}
		printf("\n");
	}
	printf("\n");
}

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

/** Updates the number of cells each player owns */
void update_num_cells(char player, int* num_player_up, int* num_player_down)
{
	if (player == '^')
	{
		*num_player_up = 0;
	}
	else if (player == 'v')
	{
		*num_player_down = 0;
	}
	
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if ((player == '^') && (get_cell(i, j) == '^'))
			{
				(*num_player_up)++;
			}
			else if ((player == 'v') && (get_cell(i, j) == 'v'))
			{
				(*num_player_down)++;
			}
		}
	}
}

/** Prints the percentage of the board each player owns */
void print_occupation(int* num_player_up, int* num_player_down)
{
	int perc_up = 100 * (*num_player_up) / (BOARD_SIZE*BOARD_SIZE);
	int perc_down = 100 * (*num_player_down) / (BOARD_SIZE*BOARD_SIZE);

	printf("PLAYER ^: %d\nPLAYER v: %d\n", perc_up, perc_down);
	printf("###########################################################\n");
}

/** Checks if the victory condition is met */
bool is_end(int* num_player_up, int* num_player_down)
{	
	int perc_up = 100 * (*num_player_up) / (BOARD_SIZE*BOARD_SIZE);
	int perc_down = 100 * (*num_player_down) / (BOARD_SIZE*BOARD_SIZE);
	
	return ((perc_up > 50) || (perc_down > 50));
}

/** Calculates how many new cells will be gained by a given player if
 * a given color is chosen.*/
int calc_new_cells(char player, char color)
{
	int num_new_cells = 0;
	
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (is_next_to_player(player, i, j) && get_cell(i, j) == color)
			{
				num_new_cells++;
			}
		}
	}
	
	return num_new_cells;
}

/** The IA acts accordingly to the best local choice. */
char IA_greedy()
{
	char color, max_color;
	int temp_cells;
	int max_cells = 0;
	
	for (color = 65; color < 72; color++)
	{
		temp_cells = calc_new_cells('v', color);
		if (temp_cells > max_cells)
		{
			max_cells = temp_cells;
			max_color = color;
		}
	}
	
	return max_color;
}

/** The IA chooses a random color*/
char IA_random()
{
	char rand_color = 65 + (rand() % 7);
	return rand_color;
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
    
    int num_start_up = 1;
    int num_start_down = 1;
    int* num_player_up = &num_start_up;
    int* num_player_down = &num_start_down;
	
	bool is_IA_turn = false;
	
	rand_board();
    print_board();
    print_occupation(num_player_up, num_player_down);
    
    // game loop
	while (!is_end(num_player_up, num_player_down))
	{
		// either gets input from player, or else IA plays
		switch (player)
		{
			case '^':
				color_input = player_input();
				is_IA_turn = false;
				break;
			
			case 'v':
				color_input = IA_greedy();
				is_IA_turn = true;
				break;
				
			default:
				printf("\nERROR\n");
		}			
		
		update_board(player, color_input);
		
		// prints the action and the result of the IA
		if (is_IA_turn)
		{
			printf("\nTHE IA HAS CHOSEN THIS COLOR: %c\n", color_input);
			print_board();
		}
		
		update_num_cells(player, num_player_up, num_player_down);
		print_occupation(num_player_up, num_player_down);
		
		player = change_player(player);
	}
	
	print_board();

    return 0; // Everything went well
}
