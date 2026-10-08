/*
 Vincent Sanchez
 Tabatha Valverde
 CSCI-4120-M01 - Operating Systems
 Modified: 7-Oct-2026
 Project 1 - Module A
*/

#include "Module_A.h"


int main(int argc, char *argv[]) {
  
    if (argc != 2) {
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        return 1;
    }

    int num_processes = atoi(argv[1]);
    if (num_processes <= 0) {
        printf("Invalid number of processes: %d\n", num_processes);
        return 1;
    }


    struct timespec start, end;
    long long seconds, nano_seconds;


    /* ToDo 1:
     * Dynamically allocate memory of size ROWS_COUNT x COLUMN_COUNT x size_of(int).
     * run "man malloc" on ubuntu/linux terminal or see online help about malloc function
     */
    int *array = malloc(ROWS_COUNT * COLUMN_COUNT * sizeof(int));
    
    // Base case
    if (array == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    } // end of if

    /* ToDo 2:
     * Initialize the allocated memory region 1 to hold
     * the matrix of size ROW_COUNT x COLUMN_COUNT.
     * Value of each element is set to i+j, where i is index of row
     * and j is index of column.
    */
    for (int i = 0; i < ROWS_COUNT; i++){
        for (int j = 0; j < COLUMN_COUNT; j++){
            array[i * COLUMN_COUNT + j] = i + j;
        }
    }


    int row_per_process=ROWS_COUNT/num_processes;
    
    clock_gettime(CLOCK_REALTIME, &start);

    
    // ToDo 3: Create num_processes count of children processes. (using fork)
    pid_t pid;

    for (int i = 0; i < num_processes; i++){
        
        int start_row = i * row_per_process;
        int end_row = start_row + row_per_process - 1;
        
        pid = fork();

        if (pid < 0) {
            
            perror("fork");
            free(array);
            return 1;
            
        } // end of if
        
        if (pid == 0){
            // ToDo 4: Ask each child process to print "row_per_process" number of rows by calling function print_matrix_rows(). See header file.
             
            print_matrix_rows(array, start_row, end_row);
             
            // ToDo 5: Each child process MUST free the dynamically allocated memory for matrix from its own adress space after printing the designated rows.
             
            free(array);
             
            // ToDo 6: All children processes MUST exit after freeing dyamically allocated memory of matrix.
            
            exit(0);
            
        } // end if
        
    } // end for loop
    

    /*ToDO 7: Parent process must wait for all children processes to complete*/
    
    for (int i = 0; i < num_processes; i++) {
        
        wait(NULL);
        
    } // end of for loop

    printf("all child processes completed their execution\n");


    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate execution time
    seconds=end.tv_sec - start.tv_sec;
    nano_seconds=end.tv_nsec - start.tv_nsec;
    long double executio_time=(long double)seconds+((long double)nano_seconds/1000000000);
    printf("Execution Time with %d process(es): %Lf\n",num_processes,executio_time);

    free (array);

    return 0;
}
