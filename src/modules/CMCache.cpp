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

res_t CMCache::accessCache(CMAddr *addr) {
  // tick the cache system time
  cacheAge++;
  dassert(cacheAge != std::numeric_limits<long long unsigned>::max(),
          "Cache age about to overflow!");

  if (isInCache(addr) != NULL) {
    return RTYPE_HIT;
  }

  bool foundSpace = bringLineIntoCache(addr);
  return foundSpace ? RTYPE_MISS : RTYPE_EVICT;
}

// If addr is in the cache, return the line it is in,
// otherwise return NULL.
CMLine *CMCache::isInCache(CMAddr *addr) {
  CMSet *set = sets.at(addr->set_index);
  return set->isInSet(addr, cacheAge);
}

bool CMCache::bringLineIntoCache(CMAddr *addr) {
  CMSet *set = sets.at(addr->set_index);
  return set->bringLineIntoSet(addr);
}

// If addr is in the cache, return the state of the line,
// otherwise return NULL.
state_t CMCache::getLineState(CMAddr *addr) {
  CMLine *line = isInCache(addr);
  return (line == NULL) ? STYPE_NONE : line->stype;
}

void CMCache::invalidate(CMAddr *addr) {
  CMLine *line = isInCache(addr);
  dassert(line != NULL, "Line is NULL");
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
