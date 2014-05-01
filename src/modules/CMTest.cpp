/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMTest.h"
#include "CMAddr.h"
#include "debug.h"

CMTest::CMTest() {
}

CMTest::~CMTest() {
  std::vector<CMAddr*>::iterator it;
  for (it = addrs.begin(); it != addrs.end(); ++it) {
    CMAddr *addr = *it;
    delete addr;
  }
}

void CMTest::addToTest(CMAddr *addr) {
  addrs.push_back(addr);
}