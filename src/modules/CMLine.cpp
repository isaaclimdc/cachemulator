/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMAddr.h"
#include "CMLine.h"
#include "debug.h"

CMLine::CMLine() {
  dirty = false;
  age = 0;
  stype = STYPE_INVALID;
  tag = 0;
}

CMLine::~CMLine() {
}

bool CMLine::isHit(CMAddr *addr, long long unsigned cacheAge) {
  if (stype != STYPE_INVALID && (addr->tag == tag)) {
    age = cacheAge;
    return true;
  } else {
    return false;
  }
}

void CMLine::update(CMAddr *addr) {
  tag = addr->tag;
  dirty = false;
  age = 0;

  if (addr->itype == ITYPE_READ) {
    stype = STYPE_SHARED;
  }
  else if (addr->itype == ITYPE_WRITE) {
    stype = STYPE_MODIFIED;
  }
  else {
    stype = STYPE_INVALID;
  }
}
