// Anonymous Pipes - Communication of 1 million integers
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define NUM_INTEGERS 1000000

int main() {
    int fd[2];
    pipe(fd);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    if (fork() == 0) { // Child (writer)
        close(fd[0]);
        
        // Send 1 million integers
        for (int i = 0; i < NUM_INTEGERS; i++) {
            write(fd[1], &i, sizeof(int));
        }
        
        close(fd[1]);
        exit(0);
        
    } else { // Parent (reader)
        close(fd[1]);
        
        int received_int;
        int count = 0;
        
        // Receive 1 million integers
        while (count < NUM_INTEGERS && read(fd[0], &received_int, sizeof(int)) > 0) {
            count++;
        }
        
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        // Calculate time difference
        double time_taken = (end.tv_sec - start.tv_sec) + 
                           (end.tv_nsec - start.tv_nsec) / 1e9;
        
        printf("Anonymous Pipe Communication Complete:\n");
        printf("- Integers sent/received: %d\n", count);
        printf("- Time taken: %.6f seconds\n", time_taken);
        
        wait(NULL); // Wait for child to finish
        close(fd[0]);
    }
    return 0;
}
