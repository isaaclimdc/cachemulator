/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "modules/CMComp.h"
#include "modules/CMCache.h"
#include "modules/CMSet.h"
#include "modules/CMAddr.h"
#include "modules/CMLine.h"
#include "modules/CMTest.h"
#include "modules/debug.h"
#include "modules/CMGlobals.h"

/* Function declarations */

void parseTraceFile(char *filePath, CMTest *test);

/* Function definitions */

int main(int argc, char **argv) {
  char *filePath = NULL;

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

  if (filePath == NULL) {
    printf("Usage: %s [-t <trace file>]\n", argv[0]);
    exit(1);
  }

  // Initialize a config singleton before doing anything else
  CONFIG = new CMConfig();

  CMTest *test = new CMTest();
  parseTraceFile(filePath, test);

  CMComp *comp = new CMComp(CONFIG->num_procs);
  comp->distrbTrace(test);

  std::vector<state_t> verif;

  while (comp->hasOutstandingJobs()) {
    // Tick computer
    comp->tick(verif);
  }

  // Check verif here
  dprintf("VERIF SIZE: %d\n", verif.size());

  delete test;
  delete comp;

  return 0;
}

void parseTraceFile(char *filePath, CMTest *test) {
  FILE *traceFile = fopen(filePath, "r");

  if (traceFile == NULL) {
    perror("Error opening file.");
    exit(1);
  }

  int max_len = CONFIG->max_trace_line_length;

  char traceLine[max_len];

  while (fgets(traceLine, max_len, traceFile) != NULL) {
    char op;
    long long unsigned rawAddr;
    size_t tid;

    sscanf(traceLine, "%c %llx %zu", &op, &rawAddr, &tid);

    inst_t itype = op == 'R' ? ITYPE_READ : ITYPE_WRITE;
    CMAddr *addr = new CMAddr(rawAddr, itype, tid);

    // if (op == 'R') {
    // }
    // else if (op == 'W') {
    // }

    test->addToTest(addr);
  }

  fclose(traceFile);
}
