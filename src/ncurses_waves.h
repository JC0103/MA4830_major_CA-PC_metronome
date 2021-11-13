#ifndef NCURSES_WAVES_H
#define NCURSES_WAVES_H

#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include "multi_thread.h"
#include "pass_arg.h"

#define DEFAULT_AMPLITUDE 10
#define DEFAULT_PERIOD .35
#define DEFAULT_DELAY_MULTIPLIER 150
#define BASE_DELAY 100

typedef unsigned short ushort;
double x; double y; int period2; double ratio; int global_period2;

unsigned int delayx;
short phase_shift;
double amplitude_thread2;
double period;
int choice, n_choices, a, p;

int title_row, title_column;

void printwave(float amplitude, float period, short phase_shift, int choice);
void ncurses_init();
void ncurses_generate_wave();
void print_menu(WINDOW *choices_win);

#endif