/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMSet.h"
#include "CMAddr.h"
#include "CMLine.h"
#include "debug.h"

CMSet::CMSet(int E) {
  for (int Ei = 0; Ei < E; Ei++) {
    CMLine *line = new CMLine();
    lines.push_back(line);
  }
}

CMSet::~CMSet() {
  lines.clear();
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

// Returns true if it finds a place to insert the new line,
// false if it evicts.
bool CMSet::bringLineIntoSet(CMAddr *addr) {
  if (isInSet(addr, 0)) {
    throw;
  }

  std::vector<CMLine*>::iterator it;
  int oldestAge = 0;
  CMLine *oldest = *lines.begin();
  for (it = lines.begin(); it != lines.end(); ++it) {
    CMLine *line = *it;
    if (!line->valid) {
      // Found a place to stick in the new line
      line->update(addr);
      return true;
    }
    else if (line->age < oldestAge) {
      oldestAge = line->age;
      oldest = line;
    }
  }

  // No place, evict oldest line
  oldest->update(addr);
  return false;
}
