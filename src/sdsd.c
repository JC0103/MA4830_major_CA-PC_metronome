#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define DEFAULT_AMPLITUDE 1
#define DEFAULT_PERIOD .35
#define DEFAULT_DELAY_MULTIPLIER 150
#define BASE_DELAY 100

//Color count
#define NUM_COLORS 6

typedef unsigned short ushort;
double x; double y; int period2; int global_period2; double ratio; bool boolean;

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
      y = sin(period*x*ratio) * 0.5;
      break; 
      
      case 2:
      // Square
      y = ((int)x % period2) < period2/2 ? 0.5:-0.5;
      break; 

      case 3:
      // Sawtooth
      y = -(((int)x % period2) - 0.5*period2) / (float)period2 ;
      break;
      
      case 4:
      // Triangle 
      y = ((int)x % (period2*2)) >= period2 ? ((period2 - (int)x % period2) - 0.5*period2) / (float)period2 : (((int)x % period2) - 0.5*period2) / (float)period2;
      break;

      default: break;
    }

    y *= amplitude; 
    y += (LINES+10)/2;

    // Print cell
    mvprintw((int)(y), (int)(x-phase_shift), " ");
    
    
  }
  return color_index;
}
unsigned int delayx;
ushort color_index;
short phase_shift;
double amplitude;
float frequency;
double period;
int choice;
int choices_menu_x = 0;
int choices_menu_y = 0;

char *choices[] = {
            "(1)Sine_Wave",
            "(2)Square_Wave",
            "(3)Sawtooth_Wave",
            "(4)Triangular_Wave",
            };

int n_choices = sizeof(choices) / sizeof(char *);
void print_menu(WINDOW *choices_win);

void print_menu(WINDOW *choices_win)
{
  int widths, heights, number_of_choice;	

	widths = 1;
	heights = 1;
	box(choices_win, 0, 0);
  attron(A_BOLD);
  mvwprintw(choices_win, 0, widths, " CHOICES: ");
  attroff(A_BOLD);
  for(number_of_choice = 0; number_of_choice < n_choices; ++number_of_choice)
	{
    mvwprintw(choices_win, heights, widths, "%s", choices[number_of_choice]);
    heights+=1;    
	}
  //wrefresh(choices_win);
}

void print_details(WINDOW *wave_details)
{
  box(wave_details, 0, 0);
  mvwprintw(wave_details, 0, 1, " WAVE_DETAILS: ");
  mvwprintw(wave_details, 1, 1, "PERIOD:  %8.2f", period);
  mvwprintw(wave_details, 2, 1, "AMPLITUDE:  %.2f", amplitude);
  mvwprintw(wave_details, 3, 1, "PHASE: %7d", phase_shift);
  mvwprintw(wave_details, 4, 1, "FREQUENCY: %.1f", frequency);
  //wrefresh(wave_details);
}

int main(int argc, char* argv[]) {

  char title[]="The HOLA Metronome\n";
  int title_row, title_column; int i; int p;
  
	WINDOW* screen = initscr();

  //start_color();
  nodelay(screen, 1);
  cbreak();
  noecho();
  curs_set(0);
  keypad(screen, TRUE);

  // Dot settings
  //attron(A_BOLD);
  attron(A_STANDOUT);

  // Wave attributes
  delayx = DEFAULT_DELAY_MULTIPLIER;
  color_index = 0;
  phase_shift = 0;
  choice = 1;
  amplitude = (LINES/2)*DEFAULT_AMPLITUDE-5; 
  frequency = 0.1; 
  period = DEFAULT_PERIOD;
  
  while (1) {

    erase();

		if (choice==1){
			p = (int)(LINES/period)*10;		
		}
		else if (choice!=1){
		  global_period2 = 40 - (int)(period *40) % 40;
			p = global_period2;
		}
		for (i=0 ; i < p; ++i){
			printwave(amplitude, period, phase_shift, color_index, choice);
		} 	

    // Display title at top row and middle column.
    getmaxyx(stdscr,title_row,title_column);
    attron(A_BOLD);
    mvprintw(0, (title_column-strlen(title))/2, "%s", title);
    attroff(A_BOLD);

    WINDOW *wave_details;
    wave_details = newwin(6, 20, 2, (title_column-20)/2);
    print_details(wave_details);
    
    WINDOW *choices_win;
    choices_win = newwin(6, 20, 2, (title_column-20)*1/5);
    print_menu(choices_win);

    mvprintw(8, title_column/2, "RUNNING: %s", choices[choice-1]);

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
        frequency += 0.1;
        break;
      case '-':
        frequency -= 0.1;
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

    print_details(wave_details);
    wrefresh(wave_details);
    print_menu(choices_win);
    wrefresh(choices_win);
    
    if (choice == 1 ){
		  delayx = ((1/ frequency) * 20000)*period;
		  usleep(delayx);
		  phase_shift += 1;
      if(phase_shift*period >= LINES){
      phase_shift = 0;
    	}
    }
    if (choice == 2 || choice == 3){
		  delayx = ((1/ frequency) * 1000000)/period2;
		  usleep(delayx);
		  phase_shift += 1;
      if(phase_shift/period2 >= LINES){
        //period = period2;
        phase_shift = 0;
      }
    }
    
    if (choice == 4 ){
		  delayx = ((1/ frequency) * 500000)/period2;
		  usleep(delayx);
		  phase_shift += 1;
      if(phase_shift/period2 >= LINES){
        //period = period2;
        phase_shift = 0;
      }
    }
}
  endwin();
  return 0; 
}