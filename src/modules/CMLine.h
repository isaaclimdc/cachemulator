/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once
#include "CMAddr.h"

class CMLine {
  private:

  public:
    CMLine();
    ~CMLine();

    bool isHit(CMAddr *addr);
    void update(CMAddr *addr);

    unsigned tag;
    bool valid;
    bool dirty;
    int age;
};
