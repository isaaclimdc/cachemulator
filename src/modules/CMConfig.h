/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

enum prot_t {
  PTYPE_MSI,
  PTYPE_MESI,
  PTYPE_MESIF
};

class CMConfig {
  private:

  public:
    CMConfig();
    ~CMConfig();

    bool hasExclusiveState();
    bool hasForwardState();

    // Counts
    int numProcs;
    int numSetBits;
    int numLines;
    int numBlockBits;

    // Protocol
    prot_t protocol;

    // Delays
    int cacheHitDelay;
    int memDelay;
    int flushAndLoadDelay;
    int upgDelay;
};
