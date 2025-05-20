#ifndef TETRIS_H
#define TETRIS_H

#define ROWS 20
#define COLS 10
#define SHAPE_SIZE 4
#define POWERUP_COOLDOWN 5

extern int board[ROWS][COLS];
extern int piece_x, piece_y;
extern int current_piece[SHAPE_SIZE][SHAPE_SIZE];
extern int score, high_score, game_over;

void init_game();
void draw_board();
void new_piece();
int check_collision(int dx, int dy);
void place_piece();
void drop_piece();
void move_piece(int dx, int dy);
void rotate_piece();
void clear_lines();
int powerup_clear_random_col();

#endif