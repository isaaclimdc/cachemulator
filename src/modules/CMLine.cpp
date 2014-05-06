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
    if (shared) {
      stype = STYPE_SHARED;
    }
    else {
      #ifdef MESI
      stype = STYPE_EXCLUSIVE; // TODO: change to exclusive
      #else
      stype = STYPE_SHARED;
      #endif
    }
  }
  else {
    stype = STYPE_MODIFIED;
    dirty = true;
  }
}
