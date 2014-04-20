/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>

class CMLine;
class CMAddr;

class CMSet {
  private:

  public:
    CMSet(int E);
    ~CMSet();

    bool isInSet(CMAddr *addr, long long unsigned cacheAge);
    void bringLineIntoSet(CMAddr *addr);

    std::vector<CMLine*> lines;
};
