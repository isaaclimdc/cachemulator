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
#define NUM_SET_BITS 4
#define NUM_LINES 2
#define NUM_BLOCK_BITS 4  //TODO: Use proper numbers here

/* Function declarations */

void parseTraceFile(char *filePath, CMTest *test);

/* Function definitions */

int main(int argc, char **argv) {
  char *filePath;

  char c;
  while ((c = getopt(argc, argv, "t:")) != -1) {
    switch (c) {
      case 't':
        filePath = optarg;
        break;
      default:
        exit(1);
    }
  }

  CMTest *test = new CMTest();
  parseTraceFile(filePath, test);

  CMCache *cache = new CMCache(NUM_SET_BITS, NUM_LINES);

  std::vector<CMAddr*>::iterator it;
  std::vector<CMAddr*> addrs = test->addrs;
  for (it = addrs.begin(); it != addrs.end(); ++it) {
    CMAddr *addr = *it;
    addr->printAddr();
    state_t stype = cache->accessCache(addr);
    cache->printSType(stype);
  }

  delete test;
  delete cache;

  return 0;
}

void parseTraceFile(char *filePath, CMTest *test) {
  FILE *traceFile = fopen(filePath, "r");

  if (traceFile == NULL) {
    perror("Error opening file.");
    exit(1);
  }

  char traceLine[MAX_TRACE_LINE_LENGTH];

  while (fgets(traceLine, MAX_TRACE_LINE_LENGTH, traceFile) != NULL) {
    char op;
    long long unsigned rawAddr;
    size_t tid;

    sscanf(traceLine, "%c %llx %zu", &op, &rawAddr, &tid);

    inst_t itype = op == 'R' ? ITYPE_READ : ITYPE_WRITE;
    CMAddr *addr = new CMAddr(rawAddr, NUM_SET_BITS, NUM_BLOCK_BITS,
                              itype, tid);

    // if (op == 'R') {
    // }
    // else if (op == 'W') {
    // }

    test->addToTest(addr);
  }

  fclose(traceFile);
}
