/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>

class CMLine;  // Forward-declare
class CMAddr;

class CMSet {
  private:
    int _E;

  public:
    CMSet(int E);
    ~CMSet();

    bool isInSet(CMAddr *addr);
    void bringLineIntoSet(CMAddr *addr);

    std::vector<CMLine*> lines;
};
