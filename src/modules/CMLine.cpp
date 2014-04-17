/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMLine.h"
#include "debug.h"

CMLine::CMLine() {
  dprintf("Initializing CMLine...\n");
}

CMLine::~CMLine() {
  dprintf("Freeing CMLine...\n");
}
bool CMLine::isHit(CMAddr *addr) {
  return (valid && (addr->tag == tag) );
}
