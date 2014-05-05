/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

class CMConfig {
  private:

  public:
    CMConfig();
    ~CMConfig();

    int numProcs;
    int numSetBits;
    int numLines;
    int numBlockBits;
    int cacheHitDelay;
    int memDelay;
    int flushAndLoadDelay;
};
