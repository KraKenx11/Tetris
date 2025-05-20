#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "tetris.h"

#define ROWS 20
#define COLS 10
#define SHAPE_SIZE 4
#define NUM_PIECES 7

int board[ROWS][COLS] = {0};
int piece_x = 3, piece_y = 0;
int score = 0;
int high_score = 0;
int game_over = 0;
time_t last_powerup_time = 0;

int current_piece[SHAPE_SIZE][SHAPE_SIZE] =
  {
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0}
  };

int tetrominoes[NUM_PIECES][SHAPE_SIZE][SHAPE_SIZE] = 
{
  // I
  {
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0},
    {0,1,0,0}
  },
  // O
  {
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
  },
  // T
  {
    {0,0,0,0},
    {1,1,1,0},
    {0,1,0,0},
    {0,0,0,0}
  },
  // S
  {
    {0,0,0,0},
    {0,1,1,0},
    {1,1,0,0},
    {0,0,0,0}
  },
  // Z
  {
    {0,0,0,0},
    {1,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
  },
  // J
  {
    {0,1,0,0},
    {0,1,0,0},
    {1,1,0,0},
    {0,0,0,0}
  },
  // L
  {
    {0,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
  }
};

void init_game()
{
  initscr();
  noecho();
  curs_set(FALSE);
  timeout(100);
  srand(time(NULL));
  FILE *f = fopen("highscore.txt", "r");
  if(f)
  {
    fscanf(f, "%d", &high_score);
    if(fclose(f) != 0)
    {
      perror("Eroare inchidere fisier highscore.");
      exit(-1);
    }
  }
  else
  {
    high_score = 0;
  }
}

void draw_board()
{
  clear();
  for(int i=0; i<ROWS; i++)
  {
    for(int j=0; j<COLS; j++)
	  {
	    mvprintw(i, j*2, board[i][j] ? "[]" : " .");
	  }
  }
  for(int i=0; i<SHAPE_SIZE; i++)
  {
    for(int j=0; j<SHAPE_SIZE; j++)
	  {
      if(current_piece[i][j])
      {
        mvprintw(piece_y + i, (piece_x + j) * 2, "[]");
      }
	  }
  }
  mvprintw(0, COLS * 2 + 2, "Score: %d", score);
  mvprintw(1, COLS * 2 + 2, "High Score: %d", high_score);
  if(game_over)
  {
    mvprintw(ROWS / 2, COLS, "===== GAME OVER =====");
    mvprintw(ROWS / 2 + 1, COLS, "===== Final score: %d", score);
    mvprintw(ROWS / 2 + 2, COLS, "===== Press any key to quit.");
  }
  mvprintw(3, COLS * 2 + 2, "A / D: Move piece");
  mvprintw(4, COLS * 2 + 2, "S: Make piece fall faster");
  mvprintw(5, COLS * 2 + 2, "W: Rotate piece");
  mvprintw(6, COLS * 2 + 2, "Q: Quit game");
  mvprintw(8, COLS * 2 + 2, "E: Power-up - Clear random column (5 seconds cooldown!)");
  refresh();
}

void new_piece()
{
  int index = rand() % NUM_PIECES;
  for(int i=0; i<SHAPE_SIZE; i++)
  {
    for(int j=0; j<SHAPE_SIZE; j++)
    {
      current_piece[i][j] = tetrominoes[index][i][j];
    }
    piece_x = 3;
    piece_y = 0;
  }
}

int check_collision(int dx, int dy)
{
  for(int i=0; i<SHAPE_SIZE; i++)
  {
    for(int j=0; j<SHAPE_SIZE; j++)
    {
      if(current_piece[i][j])
      {
        int new_x = piece_x + j + dx;
        int new_y = piece_y + i + dy;
        if(new_x < 0 || new_x >= COLS || new_y >= ROWS || board[new_y][new_x])
        {
          return 1;
        }
      }
    }
  }
  return 0;
}

void place_piece()
{
  for(int i=0; i<SHAPE_SIZE; i++)
  {
    for(int j=0; j<SHAPE_SIZE; j++)
	  {
      if(current_piece[i][j])
      {
        board[piece_y + i][piece_x + j] = 1;
      }
	  }
  }
  new_piece();
  piece_x = 3;
  piece_y = 0;
  if(check_collision(0,0))
  {
    game_over = 1;
  }
}

void drop_piece()
{
  if(!check_collision(0,1))
  {
    piece_y++;
  }
  else
  {
    place_piece();
  }
}

void move_piece(int dx, int dy)
{
  if(!check_collision(dx,dy))
  {
    piece_x = piece_x + dx;
    piece_y = piece_y + dy;
  }
}

void rotate_piece()
{
  int temp[SHAPE_SIZE][SHAPE_SIZE] = {0};
  for(int i=0; i<SHAPE_SIZE; i++)
  {
    for(int j=0; j<SHAPE_SIZE; j++)
    {
      temp[j][SHAPE_SIZE-i-1] = current_piece[i][j];
    }
  }
  for(int i=0; i<SHAPE_SIZE; i++)
  {
    for(int j=0; j<SHAPE_SIZE; j++)
    {
      current_piece[i][j] = temp[i][j];
    }
  }
}

void clear_lines()
{
  for(int i=0; i<ROWS; i++)
  {
    int full = 1;
    for(int j=0; j<COLS; j++)
    {
      if(!board[i][j])
      {
        full = 0;
        break;
      }
    }
    if(full)
    {
      for(int k=i; k>0; k--)
      {
        for(int j=0; j<COLS; j++)
        {
          board[k][j] = board[k-1][j];
        }
      }
      for(int j=0; j<COLS; j++)
      {
        board[0][j] = 0;
      }
      score = score + 100;
    }
  }
}

int powerup_clear_random_col()
{
  time_t current_time = time(NULL);
  if(difftime(current_time, last_powerup_time) >= POWERUP_COOLDOWN)
  {
    int col = rand() % COLS;
    for(int i=0; i<ROWS; i++)
    {
      board[i][col] = 0;
    }
    score = score + 50;
    last_powerup_time = current_time;
    return 1;
  }
  return 0;
}