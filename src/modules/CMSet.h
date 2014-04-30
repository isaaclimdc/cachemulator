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
    CMSet();
    ~CMSet();

    bool isInSet(CMAddr *addr, long long unsigned cacheAge);
    bool bringLineIntoSet(CMAddr *addr);

    std::vector<CMLine*> lines;
};
