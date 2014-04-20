/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMSet.h"
#include "CMAddr.h"
#include "CMLine.h"
#include "debug.h"

CMSet::CMSet(int E) {
  // dprintf("Initializing CMSet...\n");
  for (int Ei = 0; Ei < E; Ei++) {
    CMLine *line = new CMLine();
    lines.push_back(line);
  }
}

CMSet::~CMSet() {
  lines.clear();
  // dprintf("Freeing CMSet...\n");
}

bool CMSet::isInSet(CMAddr *addr, long long unsigned cacheAge) {
  std::vector<CMLine*>::iterator it;
  for (it = lines.begin(); it != lines.end(); ++it) {
    CMLine *line = *it;
    if (line->isHit(addr, cacheAge))
      return true;
  }
  return false;
}

void CMSet::bringLineIntoSet(CMAddr *addr) {
  if (isInSet(addr, 0)) {
    throw;
  }

  std::vector<CMLine*>::iterator it;
  int oldestAge = 0;
  CMLine *oldest = *lines.begin();
  for (it = lines.begin(); it != lines.end(); ++it) {
    CMLine *line = *it;
    if (!line->valid) {
      line->update(addr);
      return;
    } else if (line->age < oldestAge) {
      oldestAge = line->age;
      oldest = line;
    }
  }

  oldest->update(addr);
}
