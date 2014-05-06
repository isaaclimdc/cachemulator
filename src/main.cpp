/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <algorithm>
#include <iostream>
#include <string.h>

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

void parseTraceFile(char *filePath, CMTest *test);
prot_t parseProtocol(char *optarg);

/* Function definitions */

int main(int argc, char **argv) {
  char *filePath = NULL;
  prot_t protocol = PTYPE_MSI;

  char c;
  while ((c = getopt(argc, argv, "t:p:")) != -1) {
    switch (c) {
      case 't':
        filePath = optarg;
        break;
      case 'p':
        protocol = parseProtocol(optarg);
        break;
      default:
        exit(1);
    }
  }

  if (filePath == NULL) {
    printf("Usage: %s [-t <trace file>]\n", argv[0]);
    exit(1);
  }

  // Initialize the global singletons
  CONFIG = new CMConfig();
  BUSRequests = new bool[CONFIG->numProcs]();

  CONFIG->protocol = protocol;

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

void parseTraceFile(char *filePath, CMTest *test) {
  FILE *traceFile = fopen(filePath, "r");

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

prot_t parseProtocol(char *optarg) {
  if (strcasecmp(optarg, "mesi") == 0) {
    return PTYPE_MESI;
  }
  else if (strcasecmp(optarg, "mesif") == 0) {
    return PTYPE_MESIF;
  }
  else {
    return PTYPE_MSI;
  }
}
