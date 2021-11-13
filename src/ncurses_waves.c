#include "ncurses_waves.h"
const char *choices[] = { 
                          "Sine_Wave",
                          "Square_Wave",
                          "Sawtooth_Wave",
                          "Triangular_Wave",
                        };
char title[]="The HOLA Metronome\n";

void printwave(float amplitude, float period, short phase_shift, int choice) {

  for (x=0.0; x < (COLS+phase_shift) ; x += 1.0) {
		
    // Find Y 
    ratio = (2.0*M_PI)/LINES;

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
      // Triangle 
      y = ((int)x % (period2*2)) >= period2 ? ((period2 - (int)x % period2) - 0.5*period2) / (float)period2 : (((int)x % period2) - 0.5*period2) / (float)period2;
      break;
      
      case 4:
      // Sawtooth
      y = -(((int)x % period2) - 0.5*period2) / (float)period2 ;
      break;

      default: break;
    }

    y *= amplitude; 
    y += (LINES+10)/2;

    // Print cell
    mvprintw((int)(y), (int)(x-phase_shift), " ");
  }
}

void ncurses_init(){

  // Curses init
  WINDOW* screen = initscr();

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
  phase_shift = 0;
  choice = 1;
  amplitude = (LINES/2)*DEFAULT_AMPLITUDE-5; 
  period = DEFAULT_PERIOD;
  period2 = 40 - (int)(period *40) % 40;

}



void ncurses_generate_wave() {

    erase();
    for (int i=0 ; i < period2; ++i){
      printwave(amplitude, period, phase_shift, wave_thread2);
    }

    // Display title at top row and middle column.
    getmaxyx(stdscr,title_row,title_column);
    mvprintw(0, (title_column-strlen(title))/2, "%s", title);

    mvprintw(2, (title_column-strlen(title))*2/3, "PERIOD: %8.2f", period);
    mvprintw(3, (title_column-strlen(title))*2/3, "AMPLITUDE: %.2f", amplitude);
    mvprintw(4, (title_column-strlen(title))*2/3, "PHASE: %7d", phase_shift);
    mvprintw(5, (title_column-strlen(title))*2/3, "Frequency: %.1f", freq_thread2);
    mvprintw(5, 2, "CHOICE: %s", choices[wave_thread2-1]);
refresh();


    if (wave_thread2 != 1 ){
      delayx = ((1/ freq_thread2) * 100000)/period2;
		  usleep(delayx);
		  phase_shift += 1;
      if(phase_shift/period2 >= LINES){
        phase_shift = 0;
      }
    }

    if (wave_thread2 == 1 ){
      delayx = ((1/ freq_thread2) * 100000)*period;
		  usleep(delayx);
		  phase_shift += 1;
      if(phase_shift*period >= LINES){
      phase_shift = 0;
    }
    
    
  }
}