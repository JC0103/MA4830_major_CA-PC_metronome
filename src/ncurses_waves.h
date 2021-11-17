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

// Default parameters
#define DEFAULT_AMPLITUDE 1
#define DEFAULT_PERIOD .35
#define DEFAULT_DELAY_MULTIPLIER 150
#define BASE_DELAY 100

// Global variables
typedef unsigned short ushort;
double x; double y; int period2; double ratio; int global_period2;

unsigned int delayx;
short phase_shift;
double amplitude_thread2;
double period;
int choice, n_choices, a, p;

int title_row, title_column;

/**
 * @brief Print different waveform according to the waveform choices in a frame
 * 
 * @param amplitude Amplitude of the waveforms
 * @param period period resolution (for zoom in or out horinzontal)
 * @param phase_shift amount of phase shifted
 * @param choice choices of waveforms 
 */
void printwave(float amplitude, float period, short phase_shift, int choice);

/**
 * @brief Intialise curses window and some parameters
 * 
 */
void ncurses_init();

/**
 * @brief Generate waveforms and some parameters of the wave
 * 
 */
void ncurses_generate_wave();

/**
 * @brief Print all available types of waveforms
 * 
 * @param choices_win curses window
 */
void print_menu(WINDOW *choices_win);

/**
 * @brief Print parameters of waveforms
 * 
 * @param wave_details curses window
 */
void print_details(WINDOW *wave_details);

/**
 * @brief Print the instructions to change parameters
 * 
 * @param instructions curses window
 */
void print_instructions(WINDOW *instructions);

// Main window
WINDOW* screen;
// Sub windows
WINDOW *wave_details;
WINDOW *choices_win;
WINDOW *instructions;

#endif