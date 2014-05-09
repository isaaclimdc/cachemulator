/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMSharing.h"
#include "debug.h"
#include "CMBusShout.h"
#include "CMAddr.h"
#include <fstream>

CMSharing::CMSharing() {
}

CMSharing::~CMSharing() {
}

void CMSharing::record(CMBusShout *shout) {
  long long unsigned lineStart = shout->addr->computeLineStartAddr();

  if (!isInMap(lineStart)) {
    sharingMap[lineStart] = 0;
  }

  sharingMap[lineStart]++;

  addrAccessHistory[lineStart].push_back(shout->addr->copy());
}

bool CMSharing::isInMap(long long unsigned addr) {
  std::map<long long unsigned, long long unsigned>::iterator it;
  it = sharingMap.find(addr);

  if (it != sharingMap.end()) {
     // Element found;
     return true;
  }

  return false;
}

void CMSharing::print() {
  dprintf("{\n");
  std::map<long long unsigned, long long unsigned>::iterator it;
  for (it = sharingMap.begin(); it != sharingMap.end(); ++it) {
    dprintf("  0x%llx -> %llu\n", it->first, it->second);
  }
  dprintf("}\n");
}

void CMSharing::reportContension() {
  std::ofstream contReportFile;
  contReportFile.open("contension.report", std::ios_base::app);
  std::map<long long unsigned, std::vector<CMAddr*> >::iterator it;
  for (it = addrAccessHistory.begin(); it != addrAccessHistory.end(); ++it) {
    std::vector<CMAddr*> history = (it->second);
    std::vector<CMAddr*>::iterator it2;
    for (it2 = history.begin(); it2 != history.end(); ++it2) {
      CMAddr * addr = *it2;
      addr->print();
    }
  }
  contReportFile.close();
}
