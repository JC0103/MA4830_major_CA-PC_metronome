#include "pass_arg.h"
#include "PCI_waves.h"
#include "multi_thread.h"
#include "ncurses_waves.h"
#include <signal.h>


void ctrl_c_handler( int signum ) {
	printf("Terminating program...\n");
	save_setting();
	shutdown_pci();
 	 endwin();
    // Reset back to canonical mode
    // tcsetattr( STDIN_FILENO, 0, &oldt);
   	exit(1);
}

   
int main(int argc, char **argv)
{
	int state;
	signal( SIGINT, ctrl_c_handler );
	state = parse_arg(argc, argv);
	if (state == -1) return 0;
	else if (state == 0){
		state = ask_for_default_setting();
		if (state == -1) return 0;
	}
	else if (state == 1){
		if (freq < FLOAT_EPSILON || amp < FLOAT_EPSILON){
			printf("No input for freqeuncy and/or amplitude\n");
			state = ask_for_default_setting();
		}
	}

	parse_arg_default();
	printf("Check\n");
	printf("%d %f %f \n", waveforms, freq, amp);

	pci_setup();

	// Declare threads' ID
	pthread_t thread1, thread2;
	pci_loop_finished = true; ncurses_loop_finished = true;
	printf("Start main thread with tid %ld \n", pthread_self());

	// Create thread1
    thread_create = pthread_create(&thread1, NULL, &read_input_thread_1, NULL);
     if (thread_create) { printf("ERROR; return code from pthread_create() for thread 1 is %d\n", thread_create); exit(-1); } 
	// Create thread2
	thread_create = pthread_create(&thread2, NULL, &ncurses_display_thread_2, NULL);
     if (thread_create) { printf("ERROR; return code from pthread_create() for thread 2 is %d\n", thread_create); exit(-1); } 


	while(1) {
		//Copy the wave parameters
		sleep(1);
		pthread_mutex_lock(&param_mutex);
		
		wave_main = waveforms;
		freq_main = freq;
		amp_main = amp;
		pthread_mutex_unlock(&param_mutex);

		//Copy flags
		pthread_mutex_lock(&flag_mutex);
		pci_loop_finished_main = pci_loop_finished;
		ncurses_loop_finished_main = ncurses_loop_finished;
		pthread_mutex_unlock(&flag_mutex);

		if (pci_loop_finished_main && ncurses_loop_finished_main) {
			pthread_mutex_lock(&flag_mutex);
			pci_loop_finished = false;
			pthread_mutex_unlock(&flag_mutex);

			generate_wave(wave_main, freq_main, amp_main);

			pthread_mutex_lock(&flag_mutex);
			pci_loop_finished = true;
			pthread_mutex_unlock(&flag_mutex);
		}
	}
}
