/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>
#include "modules/CMAddr.h"

class CMSet;  // Forward-declare

class CMCache {
  private:

  public:
    CMCache(int s, int E);
    ~CMCache();

    bool isInCache(CMAddr *addr);
    std::vector<CMSet*> sets;
};
