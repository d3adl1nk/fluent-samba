#ifndef IA_H
#define IA_H

int calc_new_cells(board_d* board, char player, char color);
int calc_new_cells_rec(board_d* board, board_d* board_visited, char player, char color, int i, int j);
int cal_new_cells_optimized(board_d* board, char player, char color, int i, int j);

char IA_greedy(board_d* board, char player);
char IA_random(void);

int border(board_d* board, char player);
char IA_greedy_border(board_d* board, char player);

char IA_foresighted_greedy(board_d* board, char player);
#endif /* IA_H */
