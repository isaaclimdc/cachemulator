/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMSet.h"
#include "CMLine.h"
#include "debug.h"

CMSet::CMSet(int E) : _E(E) {
  dprintf("Initializing CMSet...\n");
  for (int Ei = 0; Ei < E; Ei++) {
    CMLine *line = new CMLine();
    lines.push_back(line);
  }
}


CMSet::~CMSet() {
  lines.clear();
  dprintf("Freei CMSet...\n");
}

bool CMSet::isInSet(CMAddr *addr) {
  std::vector<CMLine*>::iterator it;
  for (it = lines.begin(); it != lines.end(); ++it) {
    CMLine *line = *it;
    if (line->isHit(addr))
      return true;
  }
  return false;
}

void CMSet::bringLineIntoSet(CMAddr *addr) {
  if (isInSet(addr)) {
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
    } else if (line->age > oldestAge) {
      oldestAge = line->age;
      oldest = line;
    }
  }
  oldest->update(addr);
}
