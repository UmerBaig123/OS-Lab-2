// Named Pipe (FIFO) Writer - Send 1 million integers
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define NUM_INTEGERS 1000000

int main() {
    int fd = open("myfifo", O_WRONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    // Send 1 million integers
    for (int i = 0; i < NUM_INTEGERS; i++) {
        write(fd, &i, sizeof(int));
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("FIFO Writer: Sent %d integers in %.6f seconds\n", NUM_INTEGERS, time_taken);
    
    close(fd);
    return 0;
}
