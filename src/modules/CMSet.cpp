/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMSet.h"
#include "CMLine.h"

CMSet::CMSet() {
  dprintf("Initializing CMSet...\n");
}

CMSet::~CMSet() {
  lines.clear();
  dprintf("Freeing CMSet...\n");
}
