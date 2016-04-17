//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University
//------------------------------------------------------------------------- 

// Condition variable exercise.
//
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;
pthread_cond_t cond;

void sender() {
  printf("Sender starts ...\n");
  sleep(1);
  pthread_mutex_lock(&lock);
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&lock);

}

void receiver() {
  printf("Receiver starts ...\n");
  pthread_mutex_lock(&lock);
  pthread_cond_wait(&cond, &lock);
  pthread_mutex_unlock(&lock);

  printf("Signal received!\n");
}

int main(int argc, char **argv) {
  pthread_t tid1, tid2;

  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&cond, NULL);
  pthread_create(&tid1, NULL, (void *)sender, NULL);
  pthread_create(&tid2, NULL, (void *)receiver, NULL);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
}
