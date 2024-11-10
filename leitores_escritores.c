#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int readers = 0, writer = 0, shared_data = 0;

void* reader(void* arg) {
    int id = *((int*)arg);
    free(arg);

    pthread_mutex_lock(&mutex);
    while (writer > 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    readers++;
    pthread_mutex_unlock(&mutex);

    printf("Reader %d: read shared data = %d\n", id, shared_data);
    sleep(1);

    pthread_mutex_lock(&mutex);
    readers--;
    if (readers == 0) {
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* writer(void* arg) {
    int id = *((int*)arg);
    free(arg);

    pthread_mutex_lock(&mutex);
    while (readers > 0 || writer > 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    writer++;
    pthread_mutex_unlock(&mutex);

    shared_data = id; // Update shared data
    printf("Writer %d: updated shared data to %d\n", id, shared_data);
    sleep(1);

    pthread_mutex_lock(&mutex);
    writer--;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t threads[10];
    for (int i = 0; i < 5; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, reader, id);
    }
    for (int i = 5; i < 10; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, writer, id);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}