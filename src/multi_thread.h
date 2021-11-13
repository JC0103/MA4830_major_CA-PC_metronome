#ifndef MULTI_THREAD_H
#define MULTI_THREAD_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>  

int thread_create;
pthread_mutex_t param_mutex;
pthread_mutex_t flag_mutex;

bool pci_loop_finished;
bool ncurses_loop_finished;
bool pci_loop_finished_main, pci_loop_finished_thread2;
bool ncurses_loop_finished_main, ncurses_loop_finished_thread2;

float freq_main, freq_thread2; 
float amp_main, amp_thread2;
int wave_main, wave_thread2;

void* read_input_thread_1 (void *arg);
void* ncurses_display_thread_2(void *arg); 

#endif