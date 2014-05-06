/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMSharing.h"
#include "debug.h"
#include "CMBusShout.h"
#include "CMAddr.h"

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