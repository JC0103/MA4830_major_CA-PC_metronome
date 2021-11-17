#ifndef MULTI_THREAD_H
#define MULTI_THREAD_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h> 

// Thread ID's
pthread_t thread1, thread2;

// Storage for checking thread create successful
int thread_create;

//3 mutexes for setting parameters, loop finsihed flag and ncurses parameters
pthread_mutex_t param_mutex, flag_mutex, ncurses_mutex;

// Loop / Cycle finished flag and their copies
int pci_loop_finished, ncurses_loop_finished;
int pci_loop_finished_main, pci_loop_finished_thread2;
int ncurses_loop_finished_main, ncurses_loop_finished_thread2;

// Setting parameters to be used in respective thread
float freq_main, freq_thread2; 
float amp_main, amp_thread2;
int wave_main, wave_thread2;

// parameters for turning off canonical terminal input
static struct termios oldt, newt;

/** 
 * Change to non-canonical terminal input.
 * In canonical mode,  read on the serial port will not return until a new line, EOF or EOL character is received
*/
void turn_off_canonical();

/**
 * Routine to start thread 1 which change parameters according to keyboard inputs
 * @param arg A dummy pointer
 */
void* read_input_thread_1 (void *arg);

/**
 * Routine to start thread 2 which display waveforms on terminal using ncurses library
 * @param arg A dummy pointer
 */
void* ncurses_display_thread_2(void *arg); 

#endif