/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMAddr.h"
#include "CMLine.h"
#include "debug.h"

CMLine::CMLine() {
  valid = false;
  dirty = false;
  age = 0;
}

CMLine::~CMLine() {
}

bool CMLine::isHit(CMAddr *addr, long long unsigned cacheAge) {
  if (valid && (addr->tag == tag)) {
    age = cacheAge;
    return true;
  } else {
    return false;
  }
}

void CMLine::update(CMAddr *addr) {
  tag = addr->tag;
  valid = true;
  dirty = false;
  age = 0;
}
