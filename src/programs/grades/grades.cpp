#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define FALSE_SHARING

#define NUM_STUDENTS 100
#define CACHE_LINE_SIZE 64

#ifdef FALSE_SHARING
#else
struct student {
  int grade;
  char pad[CACHE_LINE_SIZE-sizeof(int)];
};
#endif

#ifdef FALSE_SHARING
static int grades[NUM_STUDENTS];
#else
static struct student grades[NUM_STUDENTS];
#endif

#ifdef FALSE_SHARING
void *updateStudent(void *arg) {
  for (int j=0; j<100; j++) {
    int i = *(int *)arg;
    grades[i] = i*2+j;
  }
}
#else
void *updateStudent(void *arg) {
  for (int j=0; j<100; j++) {
    int i = *(int *)arg;
    grades[i].grade = i*2+j;
  }
}
#endif

#ifdef FALSE_SHARING
void printGrades() {
  printf("[");
  for (int i=0; i<NUM_STUDENTS; i++) {
    printf("%d", grades[i]);
    if (i < NUM_STUDENTS-1) {
      printf(", ");
    }
  }
  printf("]\n");
}
#else
void printGrades() {
  printf("[");
  for (int i=0; i<NUM_STUDENTS; i++) {
    printf("%d", grades[i].grade);
    if (i < NUM_STUDENTS-1) {
      printf(", ");
    }
  }
  printf("]\n");
}
#endif

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_STUDENTS];

#ifdef FALSE_SHARING
  for (int i=0; i<NUM_STUDENTS; i++) {
    grades[i] = 0;
  }
#else
  for (int i=0; i<NUM_STUDENTS; i++) {
    grades[i].grade = 0;
  }
#endif

  for (int i=0; i<NUM_STUDENTS; i++) {
    int *iptr = new int(i);
    pthread_create(&threads[i], 0, updateStudent, iptr);
  }

  for (int i=0; i<NUM_STUDENTS; i++) {
    pthread_join(threads[i], 0);
  }

  printGrades();

  return 0;
}
