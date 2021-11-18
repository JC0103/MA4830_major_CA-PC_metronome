#ifndef MULTI_THREAD_H
#define MULTI_THREAD_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h> 

// Thread ID's
pthread_t main_thread, thread1, thread2;

// Thread scheduling params
struct sched_param sch_params;
int t;
pthread_attr_t attr;


// Storage for checking thread create successful
int thread_create;

//2 mutexes for setting parameters and ncurses parameters
pthread_mutex_t param_mutex, ncurses_mutex;

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