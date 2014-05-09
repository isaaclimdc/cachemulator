/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>

#include "modules/CMComp.h"
#include "modules/CMProc.h"
#include "modules/CMCache.h"
#include "modules/CMSet.h"
#include "modules/CMAddr.h"
#include "modules/CMLine.h"
#include "modules/debug.h"
#include "modules/CMGlobals.h"
#include "modules/CMSharing.h"

#define MAX_TRACE_LINE_LENGTH 20

/* Function declarations */

void reportResults(CMComp *comp);
size_t parseTraceFile(char *filePath);
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

  size_t numProcs = parseTraceFile(filePath);
  CONFIG->protocol = protocol;
  CONFIG->numProcs = numProcs;

  busShoutsFile->open(FILE_BUSSHOUTS, std::ios_base::app);
  hitsMissesFile->open(FILE_HITSMISSES, std::ios_base::app);
  busTrafficFile->open(FILE_BUSTRAFFIC, std::ios_base::app);
  CMComp *comp = new CMComp(CONFIG->numProcs);

  while (comp->hasOutstandingJobs()) {
    comp->tick();
  }

  reportResults(comp);

  // Cleanup
  busShoutsFile->close();
  hitsMissesFile->close();
  busTrafficFile->close();

  delete comp;
  delete CONFIG;
  delete[] BUSRequests;
  delete busShoutsFile;
  delete hitsMissesFile;
  delete busTrafficFile;

  return 0;
}

void reportResults(CMComp *comp) {
  printf("Num ticks: %llu\n", comp->totalTicks);
  comp->sharing->reportContension();
  comp->sharing->print();
  std::ofstream finalReport;
  finalReport.open("stats.report", std::ios_base::app);
  finalReport << "============\n";
  finalReport << "ticks " << comp->totalTicks << "\n";
  finalReport << "instructions " << totalHits + totalMisses + totalEvictions << "\n";
  finalReport << "invalidations " << totalInvalidations << "\n";
  finalReport << "hits " << totalHits << "\n";
  finalReport << "misses " << totalMisses << "\n";
  finalReport << "evictions " << totalEvictions << "\n";
  finalReport << "BusRd " << totalBusRd << "\n";
  finalReport << "BusRdX " << totalBusRdX << "\n";
  finalReport << "BusWr " << totalBusWr << "\n";
  finalReport << "BusUpg " << totalBusUpg << "\n";
  finalReport << "C2C " << totalC2C << "\n";
  finalReport.close();

}

size_t parseTraceFile(char *filePath) {
  FILE *traceFile = fopen(filePath, "r");

  if (traceFile == NULL) {
    perror("Error opening file.");
    exit(1);
  }

  std::vector<std::ofstream*> traceFiles;

  char traceLine[MAX_TRACE_LINE_LENGTH];

  while (fgets(traceLine, MAX_TRACE_LINE_LENGTH, traceFile) != NULL) {
    if (traceLine[0] == '/' && traceLine[1] == '/') {
      continue;
    }
    totalInstructions++;

    char op;
    long long unsigned rawAddr;
    size_t pid;

    sscanf(traceLine, "%c %llx %zu", &op, &rawAddr, &pid);

    //'New' processor discovered, create a new file
    size_t totalProcsNow = traceFiles.size();
    if (pid >= totalProcsNow) {
      for (size_t counter=totalProcsNow; counter<=pid; ++counter) {
        std::string tmpPath = MAKE_TMP_FILEPATH(counter);
        traceFiles.push_back(new std::ofstream());
        traceFiles.at(counter)->open(tmpPath.c_str(), std::ios_base::app);
      }
    }

    std::ofstream *procFile = traceFiles.at(pid);
    (*procFile) << traceLine;
  }

  // Close tmp files
  std::vector<std::ofstream*>::iterator it;
  for (it = traceFiles.begin(); it != traceFiles.end(); ++it) {
    std::ofstream *file = *it;
    file->close();
  }

  // Close original trace file
  fclose(traceFile);

  return traceFiles.size();
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
