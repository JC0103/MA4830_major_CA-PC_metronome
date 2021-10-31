#include <stdio.h>
#include <ncurses.h>
#include <string.h>

// Printing menu for choices.
#define WIDTH 20
#define HEIGHT 8

int choices_menu_x = 0;
int choices_menu_y = 0;

char *choices[] = { 
            "Sine wave",
            "Square",
            "Triangle",
            "Sawtooth",
            };

int n_choices = sizeof(choices) / sizeof(char *);
void print_menu(WINDOW *choices_win, int highlight);

// Printing Sine Wave


int main()
{	
    /* This is title name. */
    char title[]="The Zai-Kia Metronome\n";
    char waveform[]="Here lies the waveform\n";
    char choices_title[]="Here is the choices of waveforms.\n";
    char freq_and_am[]="Frequency and amplitude input.\n";
    /* Get the row and column length*/
    int title_row, title_column;

    WINDOW *choices_win;
	int highlight = 0;
	int choice;
	int c;

    /* Initialize user interface */
    initscr();
    /* Display title at top row and middle column */
    getmaxyx(stdscr,title_row,title_column);
    attron(A_BOLD);
    mvprintw(0, (title_column-strlen(title))/2, "%s", title);
    attroff(A_BOLD);

    /* Display waveform at upper-middle row and middle column */
    mvprintw(title_row/5, (title_column-strlen(waveform))/2, "%s", waveform);

    /* Display input at lower-middle row and right column */
    mvprintw(title_row*2/5, (title_column-strlen(freq_and_am))*2/4, "%s", freq_and_am);

    /* Display choices at lower-middle row and left column */
    mvprintw(title_row*2/5, (title_column-strlen(choices_title))/4, "%s", choices_title);
    /* clear(); */
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	choices_menu_x = title_column/4;
	choices_menu_y = title_row*2/5 + 1;

    choices_win = newwin(HEIGHT, WIDTH, choices_menu_y, choices_menu_x);
	keypad(choices_win, TRUE);
	/* mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice"); */
	refresh();
    while(1){
        print_menu(choices_win, highlight);
        while(1)
        {	c = wgetch(choices_win);
            switch(c)
            {	case 49:
                    choice = 1;
                    break;
                case 50:
                    choice = 2;
                    break;
                case 51:
                    choice = 3;
                    break;
                case 52:
                    choice = 4;
                    break;
                default:
                    //mvprintw(title_row*2/5 + HEIGHT + 2, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
                    //mvprintw("Please choose one of the above!");
                    refresh();
                    break;
            }
            print_menu(choices_win, highlight);
            if(choice != 0)	/* User did a choice come out of the infinite loop */
                break;
        }	
        mvprintw(title_row*2/5 + HEIGHT + 1, 0, "You chose %s\n", choices[choice - 1]);
        clrtoeol();
        refresh();
    }


	getch();
	endwin();
	return 0;
}


void print_menu(WINDOW *choices_win, int highlight)
{
	int x, y, i;	

	x = 2;
	y = 2;
	box(choices_win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(choices_win, A_REVERSE); 
			mvwprintw(choices_win, y, x, "%s", choices[i]);
			wattroff(choices_win, A_REVERSE);
		}
		else
			mvwprintw(choices_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(choices_win);
}