#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int buffer = 0;
int buffer_full = 0;

void* producer(void* arg) {
    for (int i = 1; i <= 5; i++) {
        pthread_mutex_lock(&mutex);
        while (buffer_full) {
            pthread_cond_wait(&cond, &mutex);
        }
        buffer = i; // Produz um item
        printf("Producer produced %d\n", buffer);
        buffer_full = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 1; i <= 5; i++) {
        pthread_mutex_lock(&mutex);
        while (!buffer_full) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Consumer consumed %d\n", buffer);
        buffer_full = 0;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    return 0;
}