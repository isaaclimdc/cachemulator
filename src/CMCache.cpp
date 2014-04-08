/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMCache.h"
#include <vector>
#include "modules/debug.h"
#include "modules/CMSet.h"
#include "modules/CMLine.h"

CMCache::CMCache(int s, int E) {
  dprintf("Initializing CMCache...\n");

  int S = 1 << s;

  for (int Si = 0; Si < S; Si++) {
    CMSet *set = new CMSet();

    for (int Ei = 0; Ei < E; Ei++) {
      CMLine *line = new CMLine();
      set->lines.push_back(line);
    }

    sets.push_back(set);
  }
}

CMCache::~CMCache() {
  sets.clear();
  dprintf("Freeing CMCache...\n");
}