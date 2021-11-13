#include "ncurses_waves.h"
const char *choices[] = { 
                          "Sine_Wave",
                          "Square_Wave",
                          "Sawtooth_Wave",
                          "Triangular_Wave",
                        };
char title[]="The Redy Metronome\n";


void printwave(float amplitude, float period, short phase_shift, int choice) {

  for (x=0.0; x < (COLS+phase_shift) ; x += 1.0) {
		
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
void print_menu(WINDOW *choices_win)
{
  int widths, heights, number_of_choice;	

	widths = 1;
	heights = 1;
	box(choices_win, 0, 0);
  attron(A_BOLD);
  mvwprintw(choices_win, 0, widths, " CHOICES: ");
  attroff(A_BOLD);

  n_choices = sizeof(choices) / sizeof(char *);
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
  mvwprintw(wave_details, 2, 1, "AMPLITUDE:  %.2f", amp_thread2);
  mvwprintw(wave_details, 3, 1, "PHASE: %7d", phase_shift);
  mvwprintw(wave_details, 4, 1, "FREQUENCY: %.1f", freq_thread2);
  //wrefresh(wave_details);
}

void print_instructions(WINDOW *instructions)
{
  box(instructions, 0, 0);
  mvwprintw(instructions, 0, 1, " INSTRUCTIONS: ");
  mvwprintw(instructions, 1, 1, "Press (key) to change VALUE:");
  mvwprintw(instructions, 3, 1, "    -(s) AMPLITUDE +(w)    ");
  mvwprintw(instructions, 4, 1, "    -(a) FREQUENCY +(d)    ");
  //wrefresh(wave_details);
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
  amplitude_thread2 = amp_thread2 / 100 * (LINES/2)*DEFAULT_AMPLITUDE-5; 
  period = DEFAULT_PERIOD;

}

void ncurses_generate_wave() {

  erase();
  if (choice==1){
    p = (int)(LINES/period)*10;		
  }
  else if (choice!=1){
    global_period2 = 40 - (int)(period *40) % 40;
    p = global_period2;
  }
  for (a=0 ; a < p; ++a){
    printwave(amplitude_thread2, period, phase_shift, wave_thread2);
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

  WINDOW *instructions;
  instructions = newwin(6, 30, 2, (title_column-20)*4/5);
  print_instructions(instructions);

  mvprintw(8, title_column/2, "RUNNING: %s", choices[wave_thread2-1]);

  refresh();
  print_details(wave_details);
  wrefresh(wave_details);
  print_menu(choices_win);
  wrefresh(choices_win);
  print_instructions(instructions);
  wrefresh(instructions);

  if (wave_thread2 == 1 ){
    delayx = ((1/ freq_thread2) * 20000)*period;
    usleep(delayx);
    phase_shift += 1;
    if(phase_shift*period >= LINES){
    phase_shift = 0;
    }
  }
  if (wave_thread2 == 2 || wave_thread2 == 3){
    delayx = ((1/ freq_thread2) * 1000000)/period2;
    usleep(delayx);
    phase_shift += 1;
    if(phase_shift/period2 >= LINES){
      phase_shift = 0;
    }
  }
  
  if (wave_thread2 == 4 ){
    delayx = ((1/ freq_thread2) * 500000)/period2;
    usleep(delayx);
    phase_shift += 1;
    if(phase_shift/period2 >= LINES){
      phase_shift = 0;
    }
  }
}