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

void CMLine::update(CMAddr *addr, bool shared) {
  tag = addr->tag;
  dirty = false;
  age = 0; //TODO wrong!!!

  if (addr->itype == ITYPE_READ) {
    dprintf("Loading into shared\n");
    if (shared) {
      stype = STYPE_SHARED;
    }
    else {
      stype = STYPE_SHARED; // TODO: change to exclusive
    }
  }
  else if (addr->itype == ITYPE_WRITE) {
    stype = STYPE_MODIFIED;
    dirty = true;
    dprintf("Loading into modified\n");
  }
  else {
    dassert(false, "Incorrect itype!");
    stype = STYPE_INVALID;
  }
}
