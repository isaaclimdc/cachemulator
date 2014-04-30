/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>
#include "CMCache.h"

class CMAddr;
class CMCache;

class CMProc {
  private:

  public:
    CMProc();
    ~CMProc();

    void tick(std::vector<state_t> verif);

    std::vector<CMAddr*> jobs;
};
