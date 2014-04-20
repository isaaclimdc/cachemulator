/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMCache.h"
#include <vector>
#include <limits>
#include <string>
#include "modules/debug.h"
#include "modules/CMSet.h"
#include "modules/CMLine.h"
#include "modules/CMAddr.h"

CMCache::CMCache(int s, int E) {
  // dprintf("Initializing CMCache...\n")

  cacheAge = 0;
  int S = 1 << s;

  for (int Si = 0; Si < S; Si++) {
    CMSet *set = new CMSet(E);
    sets.push_back(set);
  }
}

CMCache::~CMCache() {
  sets.clear();
  // dprintf("Freeing CMCache...\n");
}

//TODO: Proper implementation
state_t CMCache::accessCache(CMAddr *addr) {
  // tick the cache system time
  cacheAge++;
  if (cacheAge == std::numeric_limits<long long unsigned>::max()) {
    throw std::string("SYSTEM IN CRITICAL STATE...GOING TO BREEAAAKKKKK!!!!");
  }

  if (isInCache(addr)) {
    return STYPE_HIT;
  }

  bringLineIntoCache(addr);
  return STYPE_MISS;
}

bool CMCache::isInCache(CMAddr *addr) {
  CMSet *set = sets.at(addr->set_index);
  return set->isInSet(addr, cacheAge);
}

void CMCache::bringLineIntoCache(CMAddr *addr) {
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
