/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMConfig.h"

CMConfig::CMConfig() {
  numProcs = 20;
  numSetBits = 8;
  numLines = 2;
  numBlockBits = 6;

  protocol = PTYPE_MSI;

  cacheHitDelay = 4;
  memDelay = 100;
  flushAndLoadDelay = 200;
  upgDelay = 4;
  c2cDelay = 30;
}

CMConfig::~CMConfig() {
}

bool CMConfig::hasExclusiveState() {
  return (protocol == PTYPE_MESI || protocol == PTYPE_MESIF);
}

bool CMConfig::hasForwardState() {
  return (protocol == PTYPE_MESIF);
}
