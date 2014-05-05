/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMAddr.h"
#include "debug.h"
#include "CMGlobals.h"

CMAddr::CMAddr(long long unsigned rawAddr, inst_t _itype, size_t _pid,
               int _data) {
  int s = CONFIG->numSetBits;
  int b = CONFIG->numBlockBits;
  unsigned sMask = (1 << s) - 1;
  unsigned bMask = (1 << b) - 1;

  tag = rawAddr >> (s+b);
  setIndex = (rawAddr >> b) & sMask;
  blockOffset = rawAddr & bMask;
  raw = rawAddr;

  itype = _itype;
  pid = _pid;

  data = _data;
}

CMAddr *CMAddr::copy() {
  return new CMAddr(raw, itype, pid, data);
}

CMAddr::~CMAddr() {
}

void CMAddr::printAddr() {
  char type = itype == ITYPE_READ ? 'R' : 'W';

  dprintf("%c at 0x%llx on proc %d", type, raw, pid);

  if (itype == ITYPE_WRITE) {
    dprintf(" with data %d", data);
  }

  dprintf("\n");
}
