// POSIX Shared Memory + Semaphore Writer - Send 1 million integers
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define NUM_INTEGERS 1000000

int main() {
    size_t shm_size = NUM_INTEGERS * sizeof(int);
    int fd = shm_open("/myshm", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }
    
    if (ftruncate(fd, shm_size) == -1) {
        perror("ftruncate");
        close(fd);
        return 1;
    }
    
    int *ptr = (int *)mmap(0, shm_size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    sem_t *sem = sem_open("/mysem", O_CREAT, 0666, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        munmap(ptr, shm_size);
        close(fd);
        return 1;
    }
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    // Write 1 million integers to shared memory
    for (int i = 0; i < NUM_INTEGERS; i++) {
        ptr[i] = i;
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_taken = (end.tv_sec - start.tv_sec) + 
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("Shared Memory Writer: Wrote %d integers in %.6f seconds\n", NUM_INTEGERS, time_taken);
    
    sem_post(sem); // Signal that data is ready

    munmap(ptr, shm_size);
    close(fd);
    sem_close(sem);
    return 0;
}
