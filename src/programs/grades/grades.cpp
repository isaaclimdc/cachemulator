#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_STUDENTS 100;

static int grades[NUM_STUDENTS];

void *updateStudent(void *arg) {
  int i = (int)arg;
  grades[i] = i*2;
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_STUDENTS];

  for (int i=0; i<NUM_STUDENTS; i++) {
    pthread_create(&threads[i], 0, updateStudent, i);
  }

  for (int i=0; i<NUM_STUDENTS; i++) {
    pthread_join(threads[i], 0);
  }

  for (int i=0; i<NUM_STUDENTS; i++) {
    printf("%d ", grades[i]);
  }
  printf("\n");

  return 0;
}
