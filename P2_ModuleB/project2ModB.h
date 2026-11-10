/*
 Vincent Sanchez
 Tabatha Valverde
 CSCI-4120-M01 - Operating Systems
 Modified: 9-Nov-2026
 Project 2 - Module B
*/

#ifndef PROJECT2MODB
#define PROJECT2MODB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> /*for pthread library*/

int threshold;

/*=============================ToDo 1: =======================
Complete body of threshold function.
Hint: google how to pass multiple parameters to a function that is executed by a thread created thorugh pthread_create().
NOTE: A thread running this function must exit (i.e., use pthread_exit()) after exceuting the function body.*/

/* Create a struct */

struct arguments {
    unsigned char* image;
    int start_row;
    int end_row;
    int width;
};

void *threshold_operation(void *arg){
    
    struct arguments *args = (struct arguments *) arg;
    
    unsigned char *my_image = args->image;
    int start_index = (args->start_row) * (args->width);
    int end_index = (args->end_row) * (args->width);
    
    for (int i = start_index; i<end_index; i++) {
        my_image[i] = (my_image[i]>threshold) ? 255:0;
    } // end of for loop
    
    pthread_exit(NULL);
} // end of function

#endif
