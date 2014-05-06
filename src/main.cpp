/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <algorithm>
#include <cstring>
#include <iostream>

#include "modules/CMComp.h"
#include "modules/CMCache.h"
#include "modules/CMSet.h"
#include "modules/CMAddr.h"
#include "modules/CMLine.h"
#include "modules/CMTest.h"
#include "modules/debug.h"
#include "modules/CMGlobals.h"
#include "modules/CMSharing.h"

#define MAX_TRACE_LINE_LENGTH 20

/* Function declarations */

void parseTraceFile(std::string filePath, CMTest *test);
bool verifyOutput(std::string filePath, std::vector<res_t> verif);

/* Function definitions */

int main(int argc, char **argv) {
  std::string filePath;

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

  if (filePath.empty()) {
    printf("Usage: %s [-t <trace file>]\n", argv[0]);
    exit(1);
  }

  // Initialize the global singletons
  CONFIG = new CMConfig();
  BUSRequests = new bool[CONFIG->numProcs]();

  // Parse traces
  CMTest *test = new CMTest();
  parseTraceFile(filePath, test);

  CMComp *comp = new CMComp(CONFIG->numProcs);
  comp->distrbTrace(test);

  while (comp->hasOutstandingJobs()) {
    // Tick computer
    comp->tick();
  }

  dprintf("Num ticks: %llu\n", comp->totalTicks);
  comp->sharing->print();

  delete test;
  delete comp;
  delete CONFIG;
  delete[] BUSRequests;

  return 0;
}

void parseTraceFile(std::string filePath, CMTest *test) {
  FILE *traceFile = fopen(filePath.c_str(), "r");

  if (traceFile == NULL) {
    perror("Error opening file.");
    exit(1);
  }

  char traceLine[MAX_TRACE_LINE_LENGTH];

  while (fgets(traceLine, MAX_TRACE_LINE_LENGTH, traceFile) != NULL) {
    if (traceLine[0] == '/' && traceLine[1] == '/') {
      continue;
    }

    char op;
    long long unsigned rawAddr;
    size_t tid;

    sscanf(traceLine, "%c %llx %zu", &op, &rawAddr, &tid);

    inst_t itype = op == 'R' ? ITYPE_READ : ITYPE_WRITE;
    CMAddr *addr = new CMAddr(rawAddr, itype, tid);

    test->addToTest(addr);
  }

  fclose(traceFile);
}
