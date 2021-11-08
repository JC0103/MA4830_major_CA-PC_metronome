#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // A macro

void* read_input_thread_1 (void *arg){
    pthread_t tid = pthread_self();
    printf ("Start thread 1 with tid %ld \n", tid);
    //thread#1 here

    pthread_mutex_lock(&mutex);
    //access/change global variable here
    pthread_mutex_unlock(&mutex);
}

void* ncurses_display_thread_2(void *arg){
    pthread_t tid = pthread_self();
    printf ("Start thread 2 with tid %ld \n", tid);
    //thread#2 here
}

int main(void){
    pthread_t thread1, thread2;
    printf("Start main thread with tid %ld \n", pthread_self());
    pthread_create(&thread1, NULL, &read_input_thread_1, NULL);
    pthread_create(&thread2, NULL, &ncurses_display_thread_2, NULL);
    sleep(1); //Need this to prevent main function return without creating thread 2
    
}