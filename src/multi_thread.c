#include "multi_thread.h"
#include "pass_arg.h"
#include "ncurses_waves.h"

// Macro to intialise mutexes
pthread_mutex_t param_mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t ncurses_mutex = PTHREAD_MUTEX_INITIALIZER;

void turn_off_canonical(){
    // Turn off canonical mode so whenever we press a char, it is immediately available to the program
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON);  
    tcsetattr( STDIN_FILENO, 0, &newt);
}

void* read_input_thread_1 (void *arg){
    // Store the thread ID and print out 
    pthread_t tid = pthread_self();
    printf ("Start thread 1 with tid %ld \n", tid);

    // Change to non-canonical mode for reading input from keyboard without typing 'Enter' after every inputs
    turn_off_canonical();

    while(1){
        // Take user input

        switch (getchar()) {
            case KEY_UP: 
            case 'w':
            // Increase amplitude (Max at 100.0)
                pthread_mutex_lock(&param_mutex);
                if (fabs(amp - 100.0) < FLOAT_EPSILON);
                else amp += 1.0;
                pthread_mutex_unlock(&param_mutex);
                break;
            case KEY_DOWN:
            case 's':
            // Decrease amplitude (Min at 1.0)
                pthread_mutex_lock(&param_mutex);
                if (fabs(amp - 1.0) < FLOAT_EPSILON);
                else amp -= 1.0;
                pthread_mutex_unlock(&param_mutex);
                break;
            case KEY_LEFT:
            case 'a':
            // Decrease frequency (Min at 0.1 Hz)
                pthread_mutex_lock(&param_mutex);
                if (fabs(freq - 0.100) < FLOAT_EPSILON);
                else freq -= 0.1;
                pthread_mutex_unlock(&param_mutex);
                break;
            case KEY_RIGHT:
            case 'd':
            // Increase frequency (Max at 10 Hz)
                pthread_mutex_lock(&param_mutex);
                if (fabs(freq - 8.0) < FLOAT_EPSILON);
                else freq += 0.1;
                pthread_mutex_unlock(&param_mutex);
                break;
            case ' ':
            // Reset phase shift to zero
                pthread_mutex_lock(&ncurses_mutex);
                phase_shift = 0.0;
                pthread_mutex_unlock(&ncurses_mutex);
                break;
            case '=':
            // Zoom out display in horinzontal axis
                pthread_mutex_lock(&ncurses_mutex);
                period += .05;
                pthread_mutex_unlock(&ncurses_mutex);
                break;
            case '-':
            // Zoom in display in horinzontal axis
                pthread_mutex_lock(&ncurses_mutex);
                period -= .05;
                pthread_mutex_unlock(&ncurses_mutex);
                break;
            case '1':
            // Switch to sine waves
                pthread_mutex_lock(&param_mutex);
                waveforms = 1;
                pthread_mutex_unlock(&param_mutex);
                break;
            case '2':
            // Switch to square waves
                pthread_mutex_lock(&param_mutex);
                waveforms = 2;
                pthread_mutex_unlock(&param_mutex);
                break;
            case '3':
            // Switch to triangular waves
                pthread_mutex_lock(&param_mutex);
                waveforms = 3;
                pthread_mutex_unlock(&param_mutex);
                break;
            case '4':
            // Switch to sawtooth waves
                pthread_mutex_lock(&param_mutex);
                waveforms = 4;
                pthread_mutex_unlock(&param_mutex);
                break;
            default: break;
        }
    }
}

void* ncurses_display_thread_2(void *arg){
    // Store the thread ID and print out 
    pthread_t tid = pthread_self();
    printf ("Start thread 2 with tid %ld \n", tid);
    
    // Initialise and set up ncurses window 
    ncurses_init();
    while(1){
        //Copy the wave parameters
        pthread_mutex_lock(&param_mutex);
        freq_thread2 = freq;
        amp_thread2 = amp;
        wave_thread2 = waveforms;
        pthread_mutex_unlock(&param_mutex);

        // General waves and display some parameters in terminal
        ncurses_generate_wave();
    }
    // Close the ncurses window
    endwin();
}