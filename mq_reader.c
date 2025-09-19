// POSIX Message Queue Reader - Receive integers (simplified)
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

#define NUM_INTEGERS 10000  // Reduced for demo due to system limitations

int main() {
    mqd_t mq = mq_open("/myq", O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }
    
    printf("Message queue opened successfully for reading.\n");
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    int received_int;
    int count = 0;
    
    // Receive integers
    while (count < NUM_INTEGERS) {
        ssize_t bytes_received = mq_receive(mq, (char*)&received_int, sizeof(int), NULL);
        if (bytes_received > 0) {
            count++;
            
            // Print progress every 1000 integers
            if (count % 1000 == 0) {
                printf("Received %d/%d integers\n", count, NUM_INTEGERS);
            }
        } else {
            perror("mq_receive");
            break;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("Message Queue Reader: Received %d integers in %.6f seconds\n", count, time_taken);
    printf("Note: Reduced to %d integers due to POSIX message queue system limitations\n", NUM_INTEGERS);
    
    mq_close(mq);
    return 0;
}
