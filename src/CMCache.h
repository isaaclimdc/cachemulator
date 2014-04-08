/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>

class CMSet;  // Forward-declare

class CMCache {
  private:

  public:
    CMCache(int s, int E);
    ~CMCache();

    std::vector<CMSet *> sets;
};
