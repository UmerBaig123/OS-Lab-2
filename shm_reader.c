// POSIX Shared Memory + Semaphore Reader - Receive 1 million integers
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_INTEGERS 1000000

int main() {
    size_t shm_size = NUM_INTEGERS * sizeof(int);
    int fd = shm_open("/myshm", O_RDONLY, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }
    
    int *ptr = (int *)mmap(0, shm_size, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    sem_t *sem = sem_open("/mysem", 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        munmap(ptr, shm_size);
        close(fd);
        return 1;
    }
    
    sem_wait(sem); // Wait for data to be ready
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    // Read and verify 1 million integers from shared memory
    int count = 0;
    for (int i = 0; i < NUM_INTEGERS; i++) {
        if (ptr[i] == i) {
            count++;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("Shared Memory Reader: Read and verified %d integers in %.6f seconds\n", count, time_taken);

    munmap(ptr, shm_size);
    close(fd);
    sem_close(sem);
    return 0;
}
