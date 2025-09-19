// UDP Socket Client - Send 1 million integers
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define NUM_INTEGERS 1000000

int main() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9090);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    // Send 1 million integers
    for (int i = 0; i < NUM_INTEGERS; i++) {
        sendto(fd, &i, sizeof(int), 0, (struct sockaddr*)&addr, sizeof(addr));
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("UDP Client: Sent %d integers in %.6f seconds\n", NUM_INTEGERS, time_taken);
    
    return 0;
}
