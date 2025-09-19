// UNIX Domain Socket Client - Send 1 million integers
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

    connect(fd, (struct sockaddr*)&addr, sizeof(addr));

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    // Send 1 million integers
    for (int i = 0; i < NUM_INTEGERS; i++) {
        write(fd, &i, sizeof(int));
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("UNIX Socket Client: Sent %d integers in %.6f seconds\n", NUM_INTEGERS, time_taken);
    
    close(fd);
    return 0;
}
