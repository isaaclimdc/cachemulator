/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMTest.h"
#include "CMAddr.h"
#include "debug.h"

CMTest::CMTest() {
  // dprintf("Initializing CMTest...\n");
}

CMTest::~CMTest() {
  // dprintf("Freeing CMTest...\n");
  addrs.clear();
}

void CMTest::addToTest(CMAddr *addr) {
  addrs.push_back(addr);
}