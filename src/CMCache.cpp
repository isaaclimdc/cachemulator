/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMCache.h"
#include <vector>
#include "modules/debug.h"
#include "modules/CMSet.h"
#include "modules/CMLine.h"
#include "modules/CMAddr.h"

CMCache::CMCache(int s, int E) {
  dprintf("Initializing CMCache...\n");

  int S = 1 << s;

  for (int Si = 0; Si < S; Si++) {
    CMSet *set = new CMSet(E);
    sets.push_back(set);
  }
}

CMCache::~CMCache() {
  sets.clear();
  dprintf("Freeing CMCache...\n");
}

bool CMCache::isInCache(CMAddr *addr) {
  CMSet *set = sets.at(addr->set_index);
  return set->isInSet(addr);
}

void CMCache::bringLineIntoCache(CMAddr *addr) {
  CMSet *set = sets.at(addr->set_index);
  return set->bringLineIntoSet(addr);
}
