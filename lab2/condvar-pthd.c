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

  // ... add code ...

}

void receiver() {
  printf("Receiver starts ...\n");

  // ... add code ...

  printf("Signal received!\n");
}

int main(int argc, char **argv) {
  pthread_t tid1, tid2;

  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&cond, NULL);
  pthread_create(&tid1, NULL, (void *)sender, NULL);
  pthread_create(&tid2, NULL, (void *)receiver, NULL);
  pthread_join(tid1, NULL);
}
