// The sum program (Pthreads version).
//
//
#include <stdio.h>
#include <pthread.h>

#define N 100 	/* problem domain size */
#define P 10 	/* number of threads */

int sum = 0; 
pthread_mutex_t sumLock;   

int compute(int i) {
    return i*i;
}

void worker(long tid) {
  
    int i, low, high, psum;
    low = (N/P) * tid; 	/* a simplistic partition scheme */
    high = low + (N/P);
    psum = 0;
    printf("Thread id: %ld. Thread range: LOW %d HIGH %d\n",tid, low, high);
    for (i = low; i < high; i++)
        psum += compute(i);
    pthread_mutex_lock(&sumLock);
    sum += psum;
    pthread_mutex_unlock(&sumLock);
} 

int main(int argc, char **argv) {
    pthread_t thread[P];
    long k; 
    pthread_mutex_init(&sumLock, NULL);   /* initialize mutex */
    for (k=0; k<P; k++)          		/* create threads */
        pthread_create(&thread[k], NULL, (void*)worker, (void*)k);
    for (k=0; k<P; k++)          		/* join threads */
        pthread_join(thread[k], NULL);
    printf("The result sum is %d\n", sum);
}  
