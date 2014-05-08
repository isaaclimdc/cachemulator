#include <sched.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define MAXTHREADS 1000

int data[MAXTHREADS];

void *threadFunc(void * arg) {
  for (int i=0; i<100; i++) {
    int *p = (int *)arg;
    printf("p is %p\n", p);
    *p = 20;
    int x = *p;
    (void)x;
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t threads[MAXTHREADS];
  int numthreads = 20;

  printf("Creating %d threads\n", numthreads);

  for (int i = 0; i< numthreads; i++) {
    int *p = (int *)malloc(sizeof(int));
    *p = 42;
    pthread_create(&threads[i], 0, threadFunc, p);
  }

  for (int i = 0; i < numthreads; i++) {

  }

  printf("All threads joined\n");

  return 0;
}
