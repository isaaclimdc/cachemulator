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

    std::vector<CMAddr*> jobs;
};
