/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMSet.h"
#include "CMAddr.h"
#include "CMLine.h"
#include "debug.h"
#include "CMGlobals.h"

CMSet::CMSet() {
  int E = CONFIG->numLines;

  for (int Ei = 0; Ei < E; Ei++) {
    CMLine *line = new CMLine();
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
CMLine *CMSet::getLine(CMAddr *addr, long long unsigned cacheAge) {
  std::vector<CMLine*>::iterator it;
  for (it = lines.begin(); it != lines.end(); ++it) {
    CMLine *line = *it;
    if (line->isHit(addr, cacheAge))
      return line;
  }
  return NULL;
}

// Returns true iff need to evict.
bool CMSet::probeLine(CMAddr *addr) {
  dassert(getLine(addr, 0) == NULL, "Line not in cache!");

  std::vector<CMLine*>::iterator it;
  int oldestAge = 0;
  for (it = lines.begin(); it != lines.end(); ++it) {
    CMLine *line = *it;
    if (line->stype == STYPE_INVALID) {
      // Found a place to stick in the new line, so no evict.
      return false;
    }
    else if (line->age < oldestAge) {
      oldestAge = line->age;
    }
  }

  // Need to evict
  return true;
}

void CMSet::bringLineIntoSet(CMAddr *addr, bool shared,
                             long long unsigned cacheAge) {
  CMLine *line = getLine(addr, cacheAge);
  if (line != NULL) {
    line->update(addr, shared);
    return;
  }

  std::vector<CMLine*>::iterator it;
  int oldestAge = 0;
  CMLine *oldest = *lines.begin();
  for (it = lines.begin(); it != lines.end(); ++it) {
    line = *it;
    if (line->stype == STYPE_INVALID) {
      // Found a place to stick in the new line
      line->update(addr, shared);
      return;
    }
    else if (line->age < oldestAge) {
      oldestAge = line->age;
      oldest = line;
    }
  }

  // No place, evict oldest line
  oldest->update(addr, shared);
}
