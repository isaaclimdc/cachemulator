/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>

class CMSet;
class CMAddr;

class CMCache {
  private:

  public:
    CMCache(int s, int E);
    ~CMCache();

    bool isInCache(CMAddr *addr);
    void bringLineIntoCache(CMAddr *addr);

    std::vector<CMSet*> sets;
};
