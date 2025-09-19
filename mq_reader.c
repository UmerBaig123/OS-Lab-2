// POSIX Message Queue Reader - Receive 1 million integers in batches
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

#define NUM_INTEGERS 1000000
#define BATCH_SIZE 1000  // Receive 1000 integers per message

int main() {
    mqd_t mq = mq_open("/myq", O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    int batch[BATCH_SIZE];
    int total_received = 0;
    
    // Receive 1 million integers in batches
    while (total_received < NUM_INTEGERS) {
        ssize_t bytes_received = mq_receive(mq, (char*)batch, BATCH_SIZE * sizeof(int), NULL);
        if (bytes_received > 0) {
            int ints_received = bytes_received / sizeof(int);
            total_received += ints_received;
            
            // Print progress every 100,000 integers
            if (total_received % 100000 == 0) {
                printf("Received %d/%d integers\n", total_received, NUM_INTEGERS);
            }
        } else {
            perror("mq_receive");
            break;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("Message Queue Reader: Received %d integers in %.6f seconds\n", total_received, time_taken);
    
    mq_close(mq);
    return 0;
}
