// Named Pipe (FIFO) Reader - Receive 1 million integers
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define NUM_INTEGERS 1000000

int main() {
    int fd = open("myfifo", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    int received_int;
    int count = 0;
    
    // Receive 1 million integers
    while (count < NUM_INTEGERS && read(fd, &received_int, sizeof(int)) > 0) {
        count++;
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("FIFO Reader: Received %d integers in %.6f seconds\n", count, time_taken);
    
    close(fd);
    return 0;
}
