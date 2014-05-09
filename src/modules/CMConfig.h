/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

enum prot_t {
  PTYPE_MSI,
  PTYPE_MESI,
  PTYPE_MESIF
};

static const char* prot_t_strings[3] = { "MSI", "MESI", "MESIF" };

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
    int c2cDelay;
};
