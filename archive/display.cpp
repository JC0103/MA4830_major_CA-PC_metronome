#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define DEFAULT_AMPLITUDE 1
#define DEFAULT_PERIOD .35
#define DEFAULT_DELAY_MULTIPLIER 150
#define BASE_DELAY 100
#define DEFAULT_WAVE_ID 0

//Color count
#define NUM_COLORS 6

typedef unsigned short ushort;

ushort printsin(double amplitude, double period,
    short phase_shift, ushort color_index) {

  double x;
  for (x=0.0; x < (COLS+phase_shift) ; x += 1.0) {

    // Swap color
/*    color_index += 1;*/
/*    attron(COLOR_PAIR((color_index % NUM_COLORS)+1));*/
		attron(COLOR_PAIR(1));
		
    // Find Y 
    double ratio = (2.0*M_PI)/LINES;
    double y = sin(period*x*ratio);
    y *= amplitude; 
    y += LINES/2;

    // Print cell
    mvprintw((int)(y), (int)(x-phase_shift), " ");

  }

  return color_index;

}

ushort printsquare(double amplitude, double period,
    short phase_shift, ushort color_index) {

  double x;
  double prev_y;
  double i;
  for (x=0.0; x < (COLS+phase_shift) ; x += 1.0) {

    // Swap color
/*    color_index += 1;*/
/*    attron(COLOR_PAIR((color_index % NUM_COLORS)+1));*/
		attron(COLOR_PAIR(2));
		
    // Find Y 
    double ratio = (2.0*M_PI)/LINES;
    double y = (fmod((x), period)) < (period/2) ? 1 : 0;
//    printw("%f\n", ratio);
    y *= -amplitude; 
    y += LINES/2;
    
		if (prev_y!=y){
			for (i=0.0; i < 1*(-amplitude) ; i += 1.0)
			i += LINES/2;
			mvprintw((int)(i), (int)(x-phase_shift), "|");
		}

    // Print cell
    mvprintw((int)(y), (int)(x-phase_shift), " ");
		prev_y = y;
  }

  return color_index;

}

int main(int argc, char* argv[]) {

  // Curses init
  WINDOW* screen = initscr();
  start_color();
  nodelay(screen, 1);
  cbreak();
  noecho();
  curs_set(0);
  keypad(screen, TRUE);

  // Color pairs
  init_pair(1, COLOR_RED, COLOR_BLACK); 
  init_pair(2, COLOR_GREEN, COLOR_BLACK); 
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  init_pair(7, COLOR_WHITE, COLOR_BLACK);
  attron(A_BOLD);
  attron(A_STANDOUT);

  // Wave attributes
  unsigned char delayx = DEFAULT_DELAY_MULTIPLIER;
  ushort color_index = 0;
  short phase_shift = 0;
  double amplitude = (LINES/2)*DEFAULT_AMPLITUDE; 
  double period = DEFAULT_PERIOD;
  int wave_id = DEFAULT_WAVE_ID;

  while (1) {

    erase();
		
//    color_index = printwave(amplitude, period, phase_shift, color_index);

		switch(wave_id){
			case 0:
				color_index = printsin(amplitude, period, phase_shift, color_index);
				break;
			case 1:
				color_index = printsquare(amplitude, period, phase_shift, color_index);
				break;
			case 2:
				break;
			case 3:
				break;
			default:
				printw("No wave option!");
		}

    attron(COLOR_PAIR(7));
    mvprintw(0, 0, "PERIOD %.2f", period);
    mvprintw(1, 0, "AMP: %.2f", amplitude);
    mvprintw(2, 0, "PHASE: %d", phase_shift);
    mvprintw(3, 0, "DELAY: x%hu", delayx);

    refresh();		
		
    switch (getch()) {
      case KEY_UP: 
      case 'k':
        amplitude += 1.0;
        break;
      case KEY_DOWN:
      case 'j':
        amplitude -= 1.0;
        break;
      case KEY_LEFT:
      case 'h':
        period -= .05;
        break;
      case KEY_RIGHT:
      case 'l':
        period += .05;
        break;
      case ' ':
        phase_shift = 0.0;
        break;
      case '+':
        delayx += 1;
        break;
      case '-':
        delayx -= 1;
        break;
      case 'n':
      	wave_id += 1;
      	if (wave_id > 3){
      		wave_id = 0;
      	}
      	break;
      case 'q':
        endwin();
        return 0;
      default: break;
    }

    usleep(BASE_DELAY*delayx);

    phase_shift += 1; 

    if (phase_shift*period >= LINES)
      phase_shift = 0;
  }

  endwin();
  return 0; 
}







