/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>

class CMAddr;

class CMProc {
  private:

  public:
    CMProc();
    ~CMProc();

    void tick();

    std::vector<CMAddr*> jobs;
};
