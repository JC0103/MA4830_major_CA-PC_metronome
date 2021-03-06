#include "ncurses_waves.h"
const char *choices[] = { 
                          "Sine_Wave         ",
                          "Square_Wave       ",
                          "Triangular_Wave   ",
                          "Sawtooth_Wave     ",
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

  // Set up the container box
	widths = 1;
	heights = 1;
	box(choices_win, 0, 0);
  attron(A_BOLD);
  mvwprintw(choices_win, 0, widths, " CHOICES: ");
  attroff(A_BOLD);

  n_choices = sizeof(choices) / sizeof(char *);
  for(number_of_choice = 0; number_of_choice < n_choices; ++number_of_choice)
	{
    // Print all the available waveform choices
    mvwprintw(choices_win, heights, widths, "%s", choices[number_of_choice]);
    heights+=1;    
	}
}

void print_details(WINDOW *wave_details)
{
  // Set up the container box
  box(wave_details, 0, 0);

  // Print all the parameters of the waveform
  mvwprintw(wave_details, 0, 1, " WAVE_DETAILS: ");
  mvwprintw(wave_details, 1, 1, "PERIOD:  %8.2f ", period);
  mvwprintw(wave_details, 2, 1, "AMPLITUDE:  %.2f ", amp_thread2);
  mvwprintw(wave_details, 3, 1, "PHASE: %7d    ", phase_shift);
  mvwprintw(wave_details, 4, 1, "FREQUENCY: %.1f    ", freq_thread2);
}

void print_instructions(WINDOW *instructions)
{
  // Set up the container box
  box(instructions, 0, 0);

  // Print respective keys to change amplitude and frequency
  mvwprintw(instructions, 0, 1, " INSTRUCTIONS: ");
  mvwprintw(instructions, 1, 1, "Press (key) to change VALUE:");
  mvwprintw(instructions, 2, 1, "                            ");
  mvwprintw(instructions, 3, 1, "    -(s) AMPLITUDE +(w)     ");
  mvwprintw(instructions, 4, 1, "    -(a) FREQUENCY +(d)     ");
  
}

void ncurses_init(){

  // Curses init
  screen = initscr();

  // Intial setup
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
  period = DEFAULT_PERIOD;
}

void ncurses_generate_wave() {

  // Erase previous outputs
  erase();

  // Set up parameters for generate wave
  if (wave_thread2==1){
    p = (int)10/period;	

  }
  else if (wave_thread2!=1){
    global_period2 = 40 - (int)(period *40) % 40;
    p = global_period2;
  }
  amplitude_thread2 = amp_thread2/100 * ((LINES)*DEFAULT_AMPLITUDE-12); 
  for (a=0 ; a < p; ++a){
    printwave(amplitude_thread2, period, phase_shift, wave_thread2);
  }

  // Display title at top row and middle column.
  getmaxyx(stdscr,title_row,title_column);
  attron(A_BOLD);
  mvprintw(0, (title_column-strlen(title))/2, "%s", title);
  attroff(A_BOLD);

  // Set these boxes as the sub-window of the main window (screen) and their sizes
  wave_details = derwin(screen, 6, 20, 2, (title_column-20)/2);

  choices_win = derwin(screen, 6, 20, 2, (title_column-20)*1/5);

  instructions = derwin(screen,6, 30, 2, (title_column-20)*4/5);

  // Print current type of waveform
  mvprintw(0, 0, "RUNNING: %s", choices[wave_thread2-1]);

  // Print all the boxes
  print_details(wave_details);
  print_menu(choices_win);
  print_instructions(instructions);

  // Refresh the curses window
  wrefresh(screen);

  // Delay according to the frequency and the type of waveforms
  if (wave_thread2 == 1 ){
    delayx = ((1/ freq_thread2) * 20000)*period;
    usleep(delayx);
    
    phase_shift += 1;
    if(phase_shift*period >= LINES){
    phase_shift = 0;
    }
  }
  if (wave_thread2 == 2 || wave_thread2 == 4){
    delayx = ((1/ freq_thread2) * 1000000)/period2;
    usleep(delayx);
    phase_shift += 1;
    if(phase_shift/period2 >= LINES){
      phase_shift = 0;
    }
  }
  
  if (wave_thread2 == 3 ){
    delayx = ((1/ freq_thread2) * 500000)/period2;
    usleep(delayx);
    phase_shift += 1;
    if(phase_shift/period2 >= LINES){
      phase_shift = 0;
    }
  }
}
