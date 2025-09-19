// POSIX Message Queue Writer - Send 1 million integers in batches
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define NUM_INTEGERS 1000000
#define BATCH_SIZE 1000  // Send 1000 integers per message

int main() {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;  // System maximum
    attr.mq_msgsize = BATCH_SIZE * sizeof(int);
    attr.mq_curmsgs = 0;
    
    mqd_t mq = mq_open("/myq", O_CREAT | O_WRONLY, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    int batch[BATCH_SIZE];
    int total_sent = 0;
    
    // Send 1 million integers in batches
    for (int batch_num = 0; batch_num < NUM_INTEGERS / BATCH_SIZE; batch_num++) {
        // Fill batch with integers
        for (int i = 0; i < BATCH_SIZE; i++) {
            batch[i] = batch_num * BATCH_SIZE + i;
        }
        
        if (mq_send(mq, (const char*)batch, BATCH_SIZE * sizeof(int), 0) == -1) {
            perror("mq_send");
            break;
        }
        total_sent += BATCH_SIZE;
        
        // Print progress every 100,000 integers
        if (total_sent % 100000 == 0) {
            printf("Sent %d/%d integers\n", total_sent, NUM_INTEGERS);
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("Message Queue Writer: Sent %d integers in %.6f seconds\n", total_sent, time_taken);
    
    mq_close(mq);
    return 0;
}
