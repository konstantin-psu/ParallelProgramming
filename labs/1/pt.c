// The sum program (Pthreads version).
//
//
#include <stdio.h>
#include <pthread.h>

void grandson() { printf("grandson\n"); }
void son(int *ip) {
    pthread_t t3;
    pthread_create(&t3, NULL, (void *)grandson, NULL);
    printf("son %d\n", *ip);
    pthread_join(t3, NULL);
}

int main() {
    int i=1, j=2;
    pthread_t t1, t2;
    pthread_create(&t1, NULL, (void *)son, &i);
    pthread_create(&t2, NULL, (void *)son, &j);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Work done\n");
}
