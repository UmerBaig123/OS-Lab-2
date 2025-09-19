// TCP Socket Server - Receive 1 million integers
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define NUM_INTEGERS 1000000

int main() {
    int server = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(server, (struct sockaddr*)&addr, sizeof(addr));
    listen(server, 5);

    int client = accept(server, NULL, NULL);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    int received_int;
    int count = 0;
    
    // Receive 1 million integers
    while (count < NUM_INTEGERS && read(client, &received_int, sizeof(int)) > 0) {
        count++;
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("TCP Server: Received %d integers in %.6f seconds\n", count, time_taken);
    
    close(client);
    close(server);
    return 0;
}
