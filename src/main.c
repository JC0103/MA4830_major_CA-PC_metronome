#include "pass_arg.h"
#include "PCI_waves.h"
#include "multi_thread.h"
#include "ncurses_waves.h"
#include <signal.h>

// SIGINT Handler (Ctrl+C) to save setting parameters, detach PCI and close ncurses window
void ctrl_c_handler( int signum ) {
	// Turn off ncurses window
 	endwin();
	printf("Terminating program...\n");
	if ( amp != 0 && freq != 0) save_setting();
	shutdown_pci();

   	exit(1);
}
   
int main(int argc, char **argv)
{
	int state;

	// Call signal handling function when signal - SIGINT is detected
	signal( SIGINT, ctrl_c_handler );

	// Process parsed arguments
	state = parse_arg(argc, argv);

	// Invalid option or arguments and exit program
	if (state == -1) {print_instruction(); return 0;}

	// No argument parse
	else if (state == 0){
		
		// Ask user to use default setting or not
		state = ask_for_default_setting();
		// User choose not to use default setting and exit program
		if (state == -1) {print_instruction(); return 0;}
	}

	// Have argument parse
	else if (state == 1){
		// Check if there is amplitude or frequency input
		if (freq < FLOAT_EPSILON || amp < FLOAT_EPSILON){
			printf("No input for freqeuncy and/or amplitude\n");
			// Prompt to ask user use default setting or not
			state = ask_for_default_setting();
			if (state == -1) {print_instruction(); return 0;}
		}
	}

	parse_arg_default();
	printf("Check\n");
	printf("%d %f %f \n", waveforms, freq, amp);

	pci_setup();

	main_thread = pthread_self();
	printf("Start main thread with tid %ld \n", main_thread);

	// Set thread 2 priority to 15
    pthread_attr_init(&attr);
    sch_params.sched_priority = 15;
    pthread_attr_setschedparam(&attr, &sch_params);
	
	// Create thread1
    thread_create = pthread_create(&thread1, NULL, &read_input_thread_1, NULL);
     if (thread_create) { printf("ERROR; return code from pthread_create() for thread 1 is %d\n", thread_create); exit(-1); } 
	// Create thread2
	thread_create = pthread_create(&thread2, &attr, &ncurses_display_thread_2, NULL);
    if (thread_create) { printf("ERROR; return code from pthread_create() for thread 2 is %d\n", thread_create); exit(-1); } 
    
	// Set main thread priority to 15
	sch_params.sched_priority = 15;
	pthread_setschedparam(main_thread, SCHED_FIFO, &sch_params);	
	
	while(1) {
		//Copy the wave parameters
		pthread_mutex_lock(&param_mutex);
		wave_main = waveforms;
		freq_main = freq;
		amp_main = amp;
		pthread_mutex_unlock(&param_mutex);

		// Generate waveforms through PCI
		generate_wave(wave_main, freq_main, amp_main);
	}
}
