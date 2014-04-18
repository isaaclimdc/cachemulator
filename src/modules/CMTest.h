/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>

class CMAddr;

class CMTest {
  private:

  public:
    CMTest();
    ~CMTest();

    void addToTest(CMAddr *addr);

    std::vector<CMAddr*> addrs;
};
