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

  std::vector<res_t> verif;

  while (comp->hasOutstandingJobs()) {
    // Tick computer
    comp->tick(verif);
  }

#ifdef DEBUG
  // Check verif here
  if (verifyOutput(filePath, verif)) {
    dprintf("TEST PASSED!\n");
  }
  else {
    dprintf("TEST FAILED...\n");
  }
#endif

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
    // if (traceLine[0] == '/' && traceLine[1] == '/') {
    //   continue;
    // }

    long long unsigned rawAddr;
    size_t tid;
    int data = 0;
    inst_t itype;

    if (traceLine[0] == 'W') {
      sscanf(traceLine, "W %llx %zu %d", &rawAddr, &tid, &data);
      itype = ITYPE_WRITE;
    }
    else {
      sscanf(traceLine, "R %llx %zu", &rawAddr, &tid);
      itype = ITYPE_READ;
    }

    CMAddr *addr = new CMAddr(rawAddr, itype, tid, data);

    test->addToTest(addr);
  }

  fclose(traceFile);
}

bool verifyOutput(std::string filePath, std::vector<res_t> verif) {
  std::vector<res_t> check;

  if (filePath.compare("traces/easy1.trace") == 0) {
    check.push_back(RTYPE_MISS);
    check.push_back(RTYPE_MISS);
    check.push_back(RTYPE_MISS);
    check.push_back(RTYPE_HIT);
    check.push_back(RTYPE_HIT);
    check.push_back(RTYPE_MISS);
    check.push_back(RTYPE_HIT);
    check.push_back(RTYPE_HIT);
    check.push_back(RTYPE_HIT);
    check.push_back(RTYPE_HIT);
  }
  else if (filePath.compare("traces/evict1.trace") == 0) {
    check.push_back(RTYPE_MISS);
    check.push_back(RTYPE_MISS);
    check.push_back(RTYPE_HIT);
    check.push_back(RTYPE_HIT);
    check.push_back(RTYPE_EVICT);
    check.push_back(RTYPE_EVICT);
  }
  else if (filePath.compare("traces/coherent1.trace") == 0) {
    check.push_back(RTYPE_MISS);
    check.push_back(RTYPE_MISS);
    check.push_back(RTYPE_MISS);
    check.push_back(RTYPE_HIT);
    check.push_back(RTYPE_HIT);
    check.push_back(RTYPE_HIT);
  }

  if (check.size() != verif.size()) {
    return false;
  }

  return std::equal(verif.begin(), verif.begin()+verif.size(), check.begin());
}
