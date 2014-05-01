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
  int S = 1 << CONFIG->num_set_bits;

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

state_t CMCache::accessCache(CMAddr *addr) {
  // tick the cache system time
  cacheAge++;
  if (cacheAge == std::numeric_limits<long long unsigned>::max()) {
    throw std::string("SYSTEM IN CRITICAL STATE...GOING TO BREEAAAKKKKK!!!!");
  }

  if (isInCache(addr)) {
    return STYPE_HIT;
  }

  bool foundSpace = bringLineIntoCache(addr);
  return foundSpace ? STYPE_MISS : STYPE_EVICT;
}

bool CMCache::isInCache(CMAddr *addr) {
  CMSet *set = sets.at(addr->set_index);
  return set->isInSet(addr, cacheAge);
}

// Returns a MISS or EVICT for that line
bool CMCache::bringLineIntoCache(CMAddr *addr) {
  CMSet *set = sets.at(addr->set_index);
  return set->bringLineIntoSet(addr);
}

void CMCache::printSType(state_t stype) {
  if (stype == STYPE_HIT) {
    dprintf("HIT\n");
  }
  else if (stype == STYPE_MISS) {
    dprintf("MISS\n");
  }
  else if (stype == STYPE_EVICT) {
    dprintf("EVICT\n");
  }
}
