#ifndef IA_H
#define IA_H

int calc_new_cells(board_d* board, char player, char color);
char IA_greedy(board_d* board);
char IA_random(void);

#endif /* IA_H */
