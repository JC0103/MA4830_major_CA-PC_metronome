#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define DEFAULT_AMPLITUDE 1
#define DEFAULT_PERIOD .35
#define DEFAULT_DELAY_MULTIPLIER 150
#define BASE_DELAY 1000

//Color count
#define NUM_COLORS 6

typedef unsigned short ushort;
double x; double y; int period2; double ratio;

ushort printwave(double amplitude, double period,
    short phase_shift, ushort color_index, int choice) {

  for (x=0.0; x < (COLS+phase_shift) ; x += 1.0) {

    // Swap color
    color_index += 1;
    // attron(COLOR_PAIR((color_index % NUM_COLORS)+1));
		
    // Find Y 
    ratio = (2.0*M_PI)/LINES;
    
    period2 = 40 - (int)(period *40) % 40;

    switch (choice){
      case 1:
      // Sine 
      y = sin(period*x*ratio);
      break; 
      
      case 2:
      // Square
      y = ((int)x % period2) < period2/2 ? ratio:-ratio;
      break; 

      case 3:
      // Sawtooth
      y = -(((int)x % period2) - 0.5*period2) / (float)period2 ;
      break;
      
      case 4:
      // Triangle 
      y = ((int)x % (period2*2)) >= period2 ? ((period2 - (int)x % period2) - 0.5*period2) / (float)period2 : (((int)x % period2) - 0.5*period2) / (float)period2;
      break;

      default: break; // TODO: PAss exception
    }

    y *= amplitude; 
    y += LINES/2;

    // Print cell
    mvprintw((int)(y), (int)(x-phase_shift), " ");
  }

  return color_index;

}
unsigned char delayx;
ushort color_index;
short phase_shift;
double amplitude;
double period;
int choice;

int main(int argc, char* argv[]) {

  // Curses init
  WINDOW* screen = initscr();
  start_color();
  nodelay(screen, 1);
  cbreak();
  noecho();
  curs_set(0);
  keypad(screen, TRUE);

  // Dot settings
  attron(A_BOLD);
  attron(A_STANDOUT);

  // Wave attributes
  delayx = DEFAULT_DELAY_MULTIPLIER;
  color_index = 0;
  phase_shift = 0;
  choice = 3;
  amplitude = (LINES/2)*DEFAULT_AMPLITUDE; 
  period = DEFAULT_PERIOD;
  while (1) {

    erase();
    printwave(amplitude, period, phase_shift, color_index, choice);

    
    mvprintw(0, 0, "PERIOD %.2f", period);
    mvprintw(1, 0, "AMP: %.2f", amplitude);
    mvprintw(2, 0, "PHASE: %d", phase_shift);
    mvprintw(3, 0, "DELAY: x%hu", delayx);
    mvprintw(4, 0, "CHOICE: %d", choice);

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
      case '1':
        choice = 1;
        break;
      case '2':
        choice = 2;
        break;
      case '3':
        choice = 3;
        break;
      case '4':
        choice = 4;
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