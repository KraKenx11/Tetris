#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#define ROWS 20
#define COLS 10
#define SHAPE_SIZE 4

int board[ROWS][COLS] = {0};
int piece_x = 3, piece_y = 0;
int current_piece[SHAPE_SIZE][SHAPE_SIZE] =
  {
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0}
  };

void init_game()
{
  initscr();
  noecho();
  curs_set(FALSE);
  timeout(100);
  srand(time(NULL));
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
  refresh();
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
  piece_x = 3;
  piece_y = 0;
}


int main()
{
  init_game();
  while(1)
    {
      draw_board();
      //drop_piece();
      //clear_lines();
      int ch = getch();
      switch(ch)
	{
	  //case 'a': move_piece(); break;
	  //case 'd': move_piece(); break;
	  //case 's': drop_piece(); break;
	  //case 'w': rotate_piece(); break;
	case 'q': endwin(); return 0;
	}
    }
  endwin();
  return 0;
}
