/*
 Vincent Sanchez
 Tabatha Valverde
 CSCI-4120-M01 - Operating Systems
 Modified: 18-Oct-2026
 Project 1 - Module B
*/

#include "Project1_Module_B.h"

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


    /* ToDo1
     * Create and attach shared memory region 1 to store the matrix
     * 
     * Hint: Google shmget() and shmat()
     *
     * shmget(): Create the region and return its shared memory id.
     * shmat(): Attach the region to this process's address space so the parent can initialize and access the matrix.
     */

    int shmid_matrix = shmget(IPC_PRIVATE, ROWS_COUNT * COLUMN_COUNT * sizeof(int), IPC_CREAT | 0600);
    
    // Stop if the matrix shared memory could not be created.
    if (shmid_matrix == -1) {
        perror("shmget matrix");
        return 1;
    } // end of if
    
    int *matrix = (int*) shmat(shmid_matrix, NULL, 0);
    
    // If attachment fails, remove the shared memory region before exiting.
    if (matrix == (void*) - 1) {
        perror("shmat matrix");
        shmctl(shmid_matrix, IPC_RMID, NULL);
        return 1;
    } // end of if
    
    
    /* ToDo2
     * create and attach shared memory region 2 to store the partial sums that
     * are returned by children processes.
     */

    int shmid_partial_sum = shmget(IPC_PRIVATE, (size_t)num_processes * sizeof(long), IPC_CREAT | 0600);
    
    // If creation fails, detahc and remove the matrix shared memory.
    if (shmid_partial_sum == -1) {
        perror("shmget partial sums");
        shmdt(matrix);
        shmctl(shmid_matrix, IPC_RMID, NULL);
        return 1;
    } // end of if
    
    // Attach the partial sums shared memory to the parent process.
    long *partial_sums = (long*) shmat(shmid_partial_sum, NULL, 0);
    
    // If attachment fails, clean up both shared memory regions.
    if (partial_sums == (void*) - 1) {
        perror("shmat partial sums");
        shmdt(matrix);
        shmctl(shmid_matrix, IPC_RMID, NULL);
        shmctl(shmid_partial_sum, IPC_RMID, NULL);
        return 1;
    } // end of if


    /* ToDo3
     * Initialize the shared memory region 1 to hold
     * the matrix of size ROW_COUNT x COLUMN_COUNT.
     * Value of each element is set to i+j, where i is index of row
     * and j is index of column.
     * 
     * Hint: Treat starting address of shared memory region as first
     * elemnt of a 2D matrix i.e.,  matrix[0][0]
    */

    for (int i = 0; i < ROWS_COUNT; i++){
        
        for (int j = 0; j < COLUMN_COUNT; j++){
            matrix[i * COLUMN_COUNT + j] = i + j;
        } // end inner for loop
        
    } // end outer for loop
    
    for (int i = 0; i < num_processes; i++){
        partial_sums[i] = 0;
    } // end for loop
    
    clock_gettime(CLOCK_REALTIME, &start);


    /* ToDo4:
     * 1) Create num_processes count of children processes. (using fork). Note that each child process runs a new binary 
     * (i.e., binary of child_proc.c) using execlp()
     * 2) Specify for each process the start_row, end_row, and
     * index in shared memory region 2 where a child process is supposed to store its
     * calculated partial sum.
     *
     *
     * Hints:
     *
     * Google "man fork" and "man execlp"
     *
    */

    int rows_per_process = ROWS_COUNT / num_processes;
    int extra_rows = ROWS_COUNT % num_processes;
    int current_row = 0;

    for (int i = 0; i < num_processes; i++) {
        int rows_assigned = rows_per_process;

        if (i < extra_rows) {
            rows_assigned++;
        }

        int start_row = current_row;
        current_row += rows_assigned;

        pid_t pid = fork();

        if (pid == 0) {

            char start_str[32];
            char rows_str[32];
            char matrix_id_str[32];
            char sum_id_str[32];
            char index_str[32];

            snprintf(start_str, sizeof(start_str), "%d", start_row);

            snprintf(rows_str, sizeof(rows_str), "%d", rows_assigned);

            snprintf(matrix_id_str, sizeof(matrix_id_str), "%d", shmid_matrix);

            snprintf(sum_id_str, sizeof(sum_id_str), "%d", shmid_partial_sum);

            snprintf(index_str, sizeof(index_str), "%d", i);

            execlp("./child", "child",
                start_str, rows_str,
                matrix_id_str, sum_id_str,
                index_str, (char *)NULL);

            perror("execlp failed");
            _exit(1);
        }
    }

    /* ToDo5
     * Wait for all children processes to complete
    */

    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    } 

    /* ToDo6:
     * Sum all the partial-sums stored in sharred memory region 2 (by children processes)
     * and print the sum.
    */
    
    long total_sum = 0;

    for (int i = 0; i < num_processes; i++) {
        total_sum += partial_sums[i];
    } 

    printf("Total Matrix Sum: %ld\n", total_sum);


    clock_gettime(CLOCK_REALTIME, &end);
    // Calculate execution time
    seconds=end.tv_sec - start.tv_sec;
    nano_seconds=end.tv_nsec - start.tv_nsec;
    long double executio_time=(long double)seconds+((long double)nano_seconds/1000000000);
    printf("Execution Time with %d process(es): %Lf\n",num_processes,executio_time);


    /* ToDo7:
     * Detach and destroy both shared memory regions.
     * Hint:
     * Google "man shmdt" and "man shmctl"
    */

    // Detach
    shmdt(matrix);
    shmdt(partial_sums);
    
    // Destroy/Remove
    shmctl(shmid_matrix, IPC_RMID, NULL);
    shmctl(shmid_partial_sum, IPC_RMID, NULL);
    
    return 0;
}


