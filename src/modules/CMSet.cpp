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
  dprintf("Freeing CMSet...\n");
}

bool CMSet::isInSet(CMAddr *addr) {
  for (int i = 0; i < _E; i++) {
    if (lines.at(i)->isHit(addr))
      return true;
  }
  return false;
}
