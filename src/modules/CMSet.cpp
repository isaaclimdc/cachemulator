/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMSet.h"
#include "CMAddr.h"
#include "CMLine.h"
#include "debug.h"
#include "CMGlobals.h"
#include <limits>

CMSet::CMSet(int _setIdx) {
  unsigned E = (unsigned)CONFIG->numLines;

  for (unsigned Ei = 0; Ei < E; Ei++) {
    CMLine *line = new CMLine(_setIdx);
    lines.push_back(line);
  }
}

CMSet::~CMSet() {
  std::vector<CMLine*>::iterator it;
  for (it = lines.begin(); it != lines.end(); ++it) {
    CMLine *line = *it;
    delete line;
  }
}

// If addr is in the set, return the line it is in,
// otherwise return NULL.
CMLine *CMSet::getLineUpdateAge(CMAddr *addr, long long unsigned cacheAge) {
  std::vector<CMLine*>::iterator it;
  for (it = lines.begin(); it != lines.end(); ++it) {
    CMLine *line = *it;
    if (line->isHitUpdateAge(addr, cacheAge))
      return line;
  }
  return NULL;
}

CMLine *CMSet::getLine(CMAddr *addr) {
  std::vector<CMLine*>::iterator it;
  for (it = lines.begin(); it != lines.end(); ++it) {
    CMLine *line = *it;
    if (line->isHit(addr))
      return line;
  }
  return NULL;
}

// Returns the old CMAddr if need to evict, else NULL
CMAddr *CMSet::probeLine(CMAddr *addr) {
  dassert(getLine(addr) == NULL, "Line not in cache!");
  dprintf("Probing line!!\n");

  std::vector<CMLine*>::iterator it;
  int oldestAge = std::numeric_limits<int>::max();
  CMAddr *oldestAddr = NULL;
  for (it = lines.begin(); it != lines.end(); ++it) {
    CMLine *line = *it;
    if (line->stype == STYPE_INVALID) {
      // Found a place to stick in the new line, so no evict.
      dprintf("Found an empty line!!!\n");
      return NULL;
    }
    else if (line->age < oldestAge) {
      oldestAge = line->age;
      oldestAddr = line->getBaseAddr();
      dprintf("OLD ADDR STUFF STUFF %p\n", oldestAddr);
    }
  }

  // Need to evict
  return oldestAddr;
}

void CMSet::bringLineIntoSet(CMAddr *addr, bool shared,
                             long long unsigned cacheAge) {
  CMLine *line = getLineUpdateAge(addr, cacheAge);
  if (line != NULL) {
    line->update(addr, shared);
    return;
  }

  std::vector<CMLine*>::iterator it;
  int oldestAge = std::numeric_limits<int>::max();
  CMLine *oldest = *lines.begin();
  for (it = lines.begin(); it != lines.end(); ++it) {
    line = *it;
    if (line->stype == STYPE_INVALID) {
      // Found a place to stick in the new line
      line->update(addr, shared);
      return;
    }
    else if (line->age <= oldestAge) {
      dprintf("older! age: %d, %p\n", line->age, line->getBaseAddr()->raw);
      oldestAge = line->age;
      oldest = line;
    }
  }

  // No place, evict oldest line
  oldest->update(addr, shared);
}
