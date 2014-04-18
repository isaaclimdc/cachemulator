/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMAddr.h"
#include "debug.h"

CMAddr::CMAddr(long long unsigned raw_addr, int s, int b) {
  dprintf("Initializing CMAddr...\n");

  unsigned s_mask = (1 << s) - 1;
  unsigned b_mask = (1 << b) - 1;

  tag = raw_addr >> (s+b);
  set_index = (raw_addr >> b) & s_mask;
  block_offset = raw_addr & b_mask;
  raw = raw_addr;
}

CMAddr::~CMAddr() {
  dprintf("Freeing CMAddr...\n");
}

void CMAddr::printAddr() {
  dprintf("%llx\n", raw);
}