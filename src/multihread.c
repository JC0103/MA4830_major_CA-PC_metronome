#include "multi_thread.h"
#include "pass_arg.h"
#include "ncurses_waves.h"

pthread_mutex_t param_mutex = PTHREAD_MUTEX_INITIALIZER; // A macro
pthread_mutex_t flag_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ncurses_mutex = PTHREAD_MUTEX_INITIALIZER;

void turn_off_canonical(){
    // Turn off canonical mode so whenever we press a char, it is immediately available to the program
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON);  
    tcsetattr( STDIN_FILENO, 0, &newt);
}

void* read_input_thread_1 (void *arg){
    pthread_t tid = pthread_self();
    printf ("Start thread 1 with tid %ld \n", tid);
    turn_off_canonical();

    while(1){
        // Take user input

        switch (getchar()) {
            case KEY_UP: 
            case 'w':
                pthread_mutex_lock(&param_mutex);
                if (abs(amp - 100.0) < FLOAT_EPSILON) break;
                else amp += 1.0;
                pthread_mutex_unlock(&param_mutex);
                break;
            case KEY_DOWN:
            case 's':
                pthread_mutex_lock(&param_mutex);
                if (abs(amp - 1.0) < FLOAT_EPSILON) break;
                else amp -= 1.0;
                pthread_mutex_unlock(&param_mutex);
                break;
            case KEY_LEFT:
            case 'a':
                pthread_mutex_lock(&ncurses_mutex);
                if (abs(freq - 0.1) < FLOAT_EPSILON) break;
                else freq -= 0.1;
                pthread_mutex_unlock(&ncurses_mutex);
                break;
            case KEY_RIGHT:
            case 'd':
                pthread_mutex_lock(&ncurses_mutex);
                if (abs(freq - 10.0) < FLOAT_EPSILON) break;
                else freq += 0.1;
                pthread_mutex_unlock(&ncurses_mutex);
                break;
            case ' ':
                pthread_mutex_lock(&ncurses_mutex);
                phase_shift = 0.0;
                pthread_mutex_unlock(&ncurses_mutex);
                break;
            case '=':
                pthread_mutex_lock(&ncurses_mutex);
                period += .05;
                pthread_mutex_unlock(&ncurses_mutex);
                break;
            case '-':
                pthread_mutex_lock(&ncurses_mutex);
                period -= .05;
                pthread_mutex_unlock(&ncurses_mutex);
                break;
            case '1':
                pthread_mutex_lock(&param_mutex);
                waveforms = 1;
                pthread_mutex_unlock(&param_mutex);
                break;
            case '2':
                pthread_mutex_lock(&param_mutex);
                waveforms = 2;
                pthread_mutex_unlock(&param_mutex);
                break;
            case '3':
                pthread_mutex_lock(&param_mutex);
                waveforms = 3;
                pthread_mutex_unlock(&param_mutex);
                break;
            case '4':
                pthread_mutex_lock(&param_mutex);
                waveforms = 4;
                pthread_mutex_unlock(&param_mutex);
                break;
            default: break;
        }
    }
}

void* ncurses_display_thread_2(void *arg){
    pthread_t tid = pthread_self();
    printf ("Start thread 2 with tid %ld \n", tid);
    
    ncurses_init();
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
        if (pci_loop_finished_thread2 && ncurses_loop_finished_thread2) {
            
			pthread_mutex_lock(&flag_mutex);
			ncurses_loop_finished = false;
			pthread_mutex_unlock(&flag_mutex);

            ncurses_generate_wave();
        
            pthread_mutex_lock(&flag_mutex);
			ncurses_loop_finished = true;
			pthread_mutex_unlock(&flag_mutex);
        }
    }
    endwin();
}