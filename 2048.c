// Library
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <curses.h>
#include "cab202_graphics.h"
// gcc 2048.c -o 2048 -std=gnu99 -Wall -Werror -IC:/cygwin64/home/ZDK -LC:/cygwin64/home/ZDK -lzdk -lncurses
// C:/Users/Viosha/Documents

// Definitions
#define CELL_X      (3)
#define CELL_Y      (3)
#define CELL_DIG    (4)
#define SCORE_UNIT  (2)
#define START_NO    (2)

// Game info
int x, y;
int cells[CELL_X*CELL_Y];
int score = 0;

// Function definitions
void set_up(void);
void game_error(int error_type);
int key_act(void);
void cell_shift(int dir);
void draw_borders(void);
void draw_cells(void);

// Main program
int main(void) {
  setup_screen();
  set_up();
  draw_borders();
  draw_cells();
  show_screen();
  wait_char();
  clear_screen();
  cleanup_screen();
  return 0;
}

// Sets up the game at start up or game over
void set_up(void) {
  // Get screen info
  int w = screen_width(), h = screen_height();

  // Calculate border constants based on cell dimensions
  x = floor((w - CELL_X - 3)/CELL_X), y = floor((h - CELL_Y - 4)/CELL_Y);

  // Cell array initialisation and clearance
  for(int i = 0; i < CELL_X*CELL_Y; i++) {
    cells[i] = 0;
  }

  //
  // for(int i = 0; i < START_NO; i++) {
  //   cells[rand()%(CELL_X*CELL_Y)] += SCORE_UNIT;
  // }
  cells[0] = SCORE_UNIT;
  cells[1] = 2*SCORE_UNIT;
  cells[2] = 3*SCORE_UNIT;
  cells[3] = 4*SCORE_UNIT;
}

// Collection of error responses
void game_error(int error_type) {
  // Screen dimensions irregular
  // TODO

  // Cell dimensions irregular
  // TODO

  // Score unit irregular
  // TODO

  // Max score unit reached
  // TODO
}

// Waits and acts on player input
int key_act(void) {
  // Waits for player input
  char key = wait_char();

  // Game responses to input
  if     (key == 'a' || key == '4') {               // Shift game LEFT
    return IN_LEFT;
  }
  else if(key == 'd' || key == '6') {               // Shift game RIGHT
    return IN_RIGHT;
  }
  else if(key == 'w' || key == '8') {               // Shift game UP
    return IN_UP;
  }
  else if(key == 's' || key == '5' || key == '2') { // Shift game DOWN
    return IN_DOWN;
  }
  else if(key == 'q') {                             // Quit game
    return QUIT;
  }
}

// Shifts cells by cycling determined cell no.s to {????}
// Also ejects quit command
void cell_shift(int dir) {
  // Key definitions          ---- MOVE ELSEWHERE ----
  #define IN_LEFT   ( 2)
  #define IN_RIGHT  ( 1)
  #define QUIT      ( 0)
  #define IN_UP     ( 3)
  #define IN_DOWN   (-3)

  // LEFT (2) & RIGHT (1) shift cycles
  if(dir == IN_LEFT || dir == IN_RIGHT) {
    for(int i = (dir - 1)*(CELL_X*CELL_Y - 1); i < CELL_X*CELL_Y && i >= 0;
        i += dir%IN_LEFT - dir + 1) {
      cells[i];
    }
  }

  // UP (3) & DOWN (-3) shift cycles
  else if(dir == IN_UP || dir == IN_DOWN) {
    for(int i = ((CELL_Y - 1) - dir*(CELL_Y - 1)/IN_UP)/2,
        l = ((CELL_X - 1) - dir*(CELL_X - 1)/IN_UP)/2; i < CELL_Y && i >= 0;
        l += dir/IN_UP) {
      if(l - i >= CELL_X || l - i < 0) {
        l = ((CELL_X - 1) - dir*(CELL_X - 1)/IN_UP)/2, i += dir/IN_UP;
      }
      cells[l*CELL_Y + i];
    }

    // // UP
    // for(int i = 0, l = 0; i >= CELL_Y; l += 1) {
    //   cells[l*CELL_Y + i];
    //   if(l - i >= CELL_X) {
    //     l = 0, i++;
    //   }
    // }
  }

  // Quit command
  else if(dir == QUIT) {
  }
}

//
int _?_(int dir, int cell) {
  if(dir == ??) {
    
  }
}

// Draws the borders & score
void draw_borders(void) {
  // Draw borders
  draw_line(0, 1, CELL_X*(x + 1) + 2, 1, '*');          // Top border
  draw_line(0, CELL_Y*(y + 1) + 3, CELL_X*(x + 1) + 2,  // Bottom border
            CELL_Y*(y + 1) + 3, '*');
  draw_line(0, 1, 0, CELL_Y*(y + 1) + 3, '*');          // Left border
  draw_line(CELL_X*(x + 1) + 2, 1, CELL_X*(x + 1) + 2,  // Right border
            CELL_Y*(y + 1) + 3, '*');

  // Draw score
  draw_formatted(0, 0, "Score: %d", score);
}

// Draws the game cells
void draw_cells(void) {
  // Draws each individual cell via loop
  int xa, xb, ya, yb;
  for(int i = 0, l = 0; i < CELL_X; l++, i += floor(l/CELL_Y), l %= CELL_Y) {
    // Omits blank 0 cells from being drawn
    if(cells[CELL_Y*i + l] > 0) { // Or == 0, if neg score_unit is wanted as a feature
      xa = x*i + i + 1, xb = x*(i + 1) + (i + 1) + 1;
      ya = y*l + l + 2, yb = y*(l + 1) + (l + 1) + 2;

      // Draws cell borders
      draw_line(xa, ya, xb, ya, '-'); // Top border
      draw_line(xa, yb, xb, yb, '-'); // Bottom border
      draw_line(xa, ya, xa, yb, '|'); // Left border
      draw_line(xb, ya, xb, yb, '|'); // Right border

      // Draws cell corners
      draw_char(xa, ya, '+');         // Top Left corner
      draw_char(xb, ya, '+');         // Top Right corner
      draw_char(xa, yb, '+');         // Bottom Left corner
      draw_char(xb, yb, '+');         // Bottom Right corner

      // Draws cell score
      draw_int((xb - xa)/2 + xa, (yb - ya)/2 + ya, cells[CELL_Y*i + l]);
    }
  }
}
