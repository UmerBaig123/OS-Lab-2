// UNIX Domain Socket Server - Receive 1 million integers
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define SOCK_PATH "unix_sock"
#define NUM_INTEGERS 1000000

int main() {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1);

    unlink(SOCK_PATH);
    bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(fd, 5);

    int client = accept(fd, NULL, NULL);
    
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
    
    printf("UNIX Socket Server: Received %d integers in %.6f seconds\n", count, time_taken);
    
    close(client);
    close(fd);
    unlink(SOCK_PATH);
    return 0;
}
