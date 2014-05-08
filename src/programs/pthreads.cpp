#include <sched.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define MAXTHREADS 1000

void *threadFunc(void *arg) {
  // for (int i=0; i<100; i++) {
    int *p = (int *)arg;
    // printf("p is %p\n", p);
    *p = 20;
    int x = *p;
    (void)x;
  // }
  return NULL;
}

int fib(int n) {
  if (n == 0 || n == 1) return 1;
  return fib(n-1) + fib(n-2);
}

void fibMemStack(int n, int *f) {
  if (n == 0 || n == 1) {
    *f = 1;
  }
  else {
    int f1, f2 = 0;
    fibMemStack(n-1, &f1);
    fibMemStack(n-2, &f2);
    *f = f1 + f2;
  }
}

void fibMemHeap(int n, int *f) {
  if (n == 0 || n == 1) {
    *f = 1;
  }
  else {
    int *f1 = (int *)malloc(sizeof(int));
    int *f2 = (int *)malloc(sizeof(int));
    fibMemHeap(n-1, f1);
    fibMemHeap(n-2, f2);
    *f = *f1 + *f2;
    free(f1); free(f2);
  }
}

int main(int argc, char *argv[]) {
  pthread_t threads[MAXTHREADS];
  int numthreads = 20;

  for (int i = 0; i< numthreads; i++) {
    int *p = (int *)malloc(sizeof(int));
    *p = 42;
    pthread_create(&threads[i], 0, threadFunc, p);
  }

  for (int i = 0; i < numthreads; i++) {
    pthread_join(threads[i], 0);
  }

  // Other stuff
  int n1 = 20;
  int f1 = fib(n1);
  printf("Fib %d is: %d\n", n1, f1);

  // int f2 = 0;
  // fibMemStack(n1, &f2);
  // printf("Fib %d is: %d\n", n1, f2);

  int *f3 = (int *)malloc(sizeof(int));
  *f3 = 0;
  fibMemHeap(n1, f3);
  printf("Fib %d is: %d\n", n1, *f3);
  free(f3);

  return 0;
}
