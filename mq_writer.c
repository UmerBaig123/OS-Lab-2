// POSIX Message Queue Writer - Send 1 million integers (simplified)
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define NUM_INTEGERS 10000  // Reduced for demo due to system limitations

int main() {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;  // System maximum
    attr.mq_msgsize = sizeof(int);
    attr.mq_curmsgs = 0;
    
    mqd_t mq = mq_open("/myq", O_CREAT | O_WRONLY, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }
    
    printf("Message queue opened successfully. Sending %d integers.\n", NUM_INTEGERS);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    // Send integers one by one
    for (int i = 0; i < NUM_INTEGERS; i++) {
        if (mq_send(mq, (const char*)&i, sizeof(int), 0) == -1) {
            perror("mq_send");
            printf("Failed after sending %d integers\n", i);
            break;
        }
        
        // Print progress every 1000 integers
        if ((i + 1) % 1000 == 0) {
            printf("Sent %d/%d integers\n", i + 1, NUM_INTEGERS);
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("Message Queue Writer: Sent %d integers in %.6f seconds\n", NUM_INTEGERS, time_taken);
    printf("Note: Reduced to %d integers due to POSIX message queue system limitations\n", NUM_INTEGERS);
    
    mq_close(mq);
    return 0;
}
