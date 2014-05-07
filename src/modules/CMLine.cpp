/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMLine.h"
#include "debug.h"
#include "CMGlobals.h"
#include "CMAddr.h"

CMLine::CMLine(unsigned _setIdx) {
  dirty = false;
  age = 0;
  stype = STYPE_INVALID;
  tag = 0;
  setIdx = _setIdx;
}

CMLine::~CMLine() {
}

bool CMLine::isHitUpdateAge(CMAddr *addr, long long unsigned cacheAge) {
  if (stype != STYPE_INVALID && (addr->tag == tag)) {
    age = cacheAge;
    return true;
  } else {
    return false;
  }
}

bool CMLine::isHit(CMAddr *addr) {
  return (stype != STYPE_INVALID) && (addr->tag == tag);
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
      if (CONFIG->hasExclusiveState()) {
        stype = STYPE_EXCLUSIVE;
      }
      else {
        stype = STYPE_SHARED;
      }
    }
  }
  else {
    stype = STYPE_MODIFIED;
    dirty = true;
  }
}

CMAddr *CMLine::getBaseAddr() {
  long long unsigned rawAddr =
    (tag << (CONFIG->numBlockBits + CONFIG->numSetBits)) |
    (setIdx << CONFIG->numBlockBits);
  return new CMAddr(rawAddr, ITYPE_READ, 0); // The pid is set later
}
