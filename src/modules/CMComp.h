/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>

class CMProc;
class CMTest;

class CMComp {
  private:

  public:
    CMComp(int P);
    ~CMComp();

    void tick();
    void distrbTrace(CMTest *test);

    std::vector<CMProc*> procs;
};
