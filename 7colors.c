/* Template of the 7 wonders of the world of the 7 colors assigment. */

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* rand */
#include <time.h>
#include <stdbool.h>

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
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", get_cell(i, j));
        }
        printf("\n");
    }
    printf("\n");
}

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

bool is_inb(int x, int y)
{
	return ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE));
}

bool is_next_to_player(char player, int x, int y)
{
	return ((is_inb(x-1, y) && (get_cell(x-1, y) == player)) ||
			(is_inb(x+1, y) && (get_cell(x+1, y) == player)) ||
			(is_inb(x, y-1) && (get_cell(x, y-1) == player)) ||
			(is_inb(x, y+1) && (get_cell(x, y+1) == player)));
}

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

char player_input(void)
{
	char color_input;
	printf("\nPLEASE ENTER A COLOR: ");
	scanf(" %c", &color_input);
	printf("\n");
	
	return color_input;
}

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

void print_occupation(int* num_player_up, int* num_player_down)
{
	int perc_up = 100 * (*num_player_up) / (BOARD_SIZE*BOARD_SIZE);
	int perc_down = 100 * (*num_player_down) / (BOARD_SIZE*BOARD_SIZE);

	printf("PLAYER ^: %d\nPLAYER v: %d\n", perc_up, perc_down);
}

bool is_end(int* num_player_up, int* num_player_down)
{	
	int perc_up = 100 * (*num_player_up) / (BOARD_SIZE*BOARD_SIZE);
	int perc_down = 100 * (*num_player_down) / (BOARD_SIZE*BOARD_SIZE);
	
	return ((perc_up > 50) || (perc_down > 50));
}

/** Program entry point */
int main(void)
{
    printf("\n\nWelcome to the 7 wonders of the world of the 7 colors\n"
	   "*****************************************************\n\n"
	   "Current board state:\n");
	
	int step_num = 0;
    char color_input, player;
    
    int num_start_up = 1;
    int num_start_down = 1;
    int* num_player_up = &num_start_up;
    int* num_player_down = &num_start_down;
	
	rand_board();
    print_board();
    print_occupation(num_player_up, num_player_down);
    
	while (!is_end(num_player_up, num_player_down))
	{
		color_input = player_input();
		player = '^' + 24 * (step_num % 2);
		
		update_board(player, color_input);
		print_board();
		update_num_cells(player, num_player_up, num_player_down);
		print_occupation(num_player_up, num_player_down);
		
		step_num++;
	}

    return 0; // Everything went well
}
