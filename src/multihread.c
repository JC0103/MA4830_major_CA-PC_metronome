#include "multi_thread.h"
#include "pass_arg.h"

pthread_mutex_t param_mutex = PTHREAD_MUTEX_INITIALIZER; // A macro
pthread_mutex_t flag_mutex = PTHREAD_MUTEX_INITIALIZER;


void* read_input_thread_1 (void *arg){
    pthread_t tid = pthread_self();
    printf ("Start thread 1 with tid %ld \n", tid);
    //thread#1 here

    pthread_mutex_lock(&param_mutex);
    //access/change global variable here
    pthread_mutex_unlock(&param_mutex);
}

void* ncurses_display_thread_2(void *arg){
    pthread_t tid = pthread_self();
    printf ("Start thread 2 with tid %ld \n", tid);

    while(1){
        //Copy the wave parameters
        pthread_mutex_lock(&param_mutex);
        freq_thread2 = freq;
        amp_thread2 = amp;
        wave_thread2 = waveforms;
        pthread_mutex_unlock(&param_mutex);

		//Copy flags
		pthread_mutex_lock(&flag_mutex);
		pci_loop_finished_thread2 = pci_loop_finished;
		ncurses_loop_finished_thread2 = ncurses_loop_finished;
		pthread_mutex_unlock(&flag_mutex);

        //thread#2 here
        if (pci_loop_finished && ncurses_loop_finished) {
            
            pthread_mutex_lock(&flag_mutex);
			ncurses_loop_finished = true;
			pthread_mutex_unlock(&flag_mutex);
        }
    }
}