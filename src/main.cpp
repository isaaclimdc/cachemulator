/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "CMCache.h"
#include "modules/CMSet.h"
#include "modules/CMAddr.h"
#include "modules/CMLine.h"
#include "modules/CMTest.h"
#include "modules/debug.h"

#define MAX_TRACE_LINE_LENGTH 20
#define NUM_SET_BITS 5
#define NUM_LINES 6
#define NUM_BLOCK_BITS 3  //TODO: Use proper numbers here

/* Function declarations */

CMTest *parseTraceFile(char *filePath);

/* Function definitions */

int main(int argc, char **argv) {
  char *filePath;

  char c;
  while ((c = getopt(argc, argv, "t")) != -1) {
    switch (c) {
      case 't':
        filePath = optarg;
        break;
      default:
        exit(1);
    }
  }

  CMTest *test = parseTraceFile(filePath);

  CMCache *cache = new CMCache(NUM_SET_BITS, NUM_LINES);

  std::vector<CMAddr*>::iterator it;
  std::vector<CMAddr*> addrs = test->addrs;
  for (it = addrs.begin(); it != addrs.end(); ++it) {
    CMAddr *addr = *it;
    addr->printAddr();
    // cache->accessCache(addr);
  }

  delete cache;
  return 0;
}

CMTest *parseTraceFile(char *filePath) {
  FILE *traceFile = fopen(filePath, "r");

  if (traceFile == NULL) {
    perror("Error opening file.");
  }

  char traceLine[MAX_TRACE_LINE_LENGTH];

  while (fgets(traceLine, MAX_TRACE_LINE_LENGTH, traceFile) != NULL) {
    char op;
    long long unsigned rawAddr;
    int tid;

    sscanf(traceLine, "%c %llx %d", &op, &rawAddr, &tid);

    CMAddr addr(rawAddr, NUM_SET_BITS, NUM_BLOCK_BITS);

    if (op == 'R') {
      dprintf("READ on thread %d!\n", tid);
    }
    else if (op == 'W') {
      dprintf("WRITE on thread %d!\n", tid);
    }
  }

  // Clean up
  fclose(traceFile);

  return 0;
}