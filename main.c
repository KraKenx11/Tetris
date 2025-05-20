#include <stdlib.h>
#include <ncurses.h>
#include "tetris.h"

int main()
{
  init_game();
  new_piece();
  while(!game_over)
  {
    draw_board();
    drop_piece();
    clear_lines();
    int ch = getch();
    switch(ch)
    {
      case 'a': move_piece(-1, 0); break;
      case 'd': move_piece(1, 0); break;
      case 's': drop_piece(); break;
      case 'w': rotate_piece(); break;
      case 'e': powerup_clear_random_col(); break;
      case 'q': game_over = 1; break;
    }
  }
  draw_board();
  timeout(-1);
  getch();
  if(score > high_score)
  {
    FILE *f = fopen("highscore.txt", "w");
    if(f)
    {
      fprintf(f, "%d", score);
      if(fclose(f) != 0)
      {
        perror("Eroare inchidere fisier highscore.");
        exit(-1);
      }
    }
    else
    {
      perror("Eroare scriere highscore in fisier.");
      exit(-1);
    }
  }
  endwin();
  return 0;
}