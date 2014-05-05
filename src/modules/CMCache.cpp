/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMCache.h"
#include <vector>
#include <limits>
#include <string>
#include <stdio.h>
#include "debug.h"
#include "CMSet.h"
#include "CMLine.h"
#include "CMAddr.h"
#include "CMGlobals.h"

CMCache::CMCache() {
  cacheAge = 0;
  int S = 1 << CONFIG->numSetBits;

  for (int Si = 0; Si < S; Si++) {
    CMSet *set = new CMSet();
    sets.push_back(set);
  }
}

CMCache::~CMCache() {
  std::vector<CMSet*>::iterator it;
  for (it = sets.begin(); it != sets.end(); ++it) {
    CMSet *set = *it;
    delete set;
  }
}

res_t CMCache::accessCache(CMAddr *addr, int &data) {
  // tick the cache system time
  cacheAge++;
  dassert(cacheAge != std::numeric_limits<long long unsigned>::max(),
          "Cache age about to overflow!");

  res_t rtype;

  if (getLine(addr) != NULL) {
    rtype = RTYPE_HIT;
  }
  else {
    bool didEvict = probeLine(addr);
    rtype = didEvict ? RTYPE_EVICT : RTYPE_MISS;
  }

  return rtype;
}

// If addr is in the cache, return the line it is in,
// otherwise return NULL.
CMLine *CMCache::getLine(CMAddr *addr) {
  CMSet *set = sets.at(addr->set_index);
  return set->getLine(addr, cacheAge);
}

bool CMCache::probeLine(CMAddr *addr) {
  CMSet *set = sets.at(addr->set_index);
  return set->probeLine(addr);
}

void CMCache::bringLineIntoCache(CMAddr *addr, bool shared) {
  CMSet *set = sets.at(addr->set_index);
  set->bringLineIntoSet(addr, shared);
}

// If addr is in the cache, return the state of the line,
// otherwise return NULL.
state_t CMCache::getLineState(CMAddr *addr) {
  CMLine *line = getLine(addr);
  return (line == NULL) ? STYPE_INVALID : line->stype;
}

void CMCache::setLineState(CMAddr *addr, state_t setToState) {
  CMLine *line = getLine(addr);
  dassert(line != NULL, "setting line state to a line not in cache");
  line->stype = setToState;
}

void CMCache::invalidate(CMAddr *addr) {
  CMLine *line = getLine(addr);
  dassert(line != NULL, "Line is NULL");
  line->stype = STYPE_INVALID;
}

void CMCache::printRType(res_t rtype) {
  if (rtype == RTYPE_HIT) {
    dprintf("HIT\n");
  }
  else if (rtype == RTYPE_MISS) {
    dprintf("MISS\n");
  }
  else if (rtype == RTYPE_EVICT) {
    dprintf("EVICT\n");
  }
}
