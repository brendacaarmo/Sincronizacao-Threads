#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_barrier_t barrier;

void* task(void* arg) {
    int id = *((int*)arg);
    free(arg);
    printf("Thread %d reached the barrier.\n", id);
    pthread_barrier_wait(&barrier);
    printf("Thread %d passed the barrier.\n", id);
    return NULL;
}

int main() {
    pthread_t threads[3];
    pthread_barrier_init(&barrier, NULL, 3);

    for (int i = 0; i < 3; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, task, id);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    return 0;
}