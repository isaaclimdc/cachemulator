/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMAddr.h"
#include "debug.h"
#include "CMGlobals.h"

CMAddr::CMAddr(long long unsigned raw_addr, inst_t _itype, size_t _pid) {
  int s = CONFIG->num_set_bits;
  int b = CONFIG->num_block_bits;
  unsigned s_mask = (1 << s) - 1;
  unsigned b_mask = (1 << b) - 1;

  tag = raw_addr >> (s+b);
  set_index = (raw_addr >> b) & s_mask;
  block_offset = raw_addr & b_mask;
  raw = raw_addr;

  itype = _itype;
  pid = _pid;
}

CMAddr::~CMAddr() {
}

void CMAddr::printAddr() {
  char type = itype == ITYPE_READ ? 'R' : 'W';
  dprintf("%c at 0x%llx on proc %d\n", type, raw, pid);
}
