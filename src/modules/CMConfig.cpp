/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMConfig.h"

CMConfig::CMConfig() {
  // TODO: parse a file to do all these settings
  numProcs = 4;
  numSetBits = 8;
  numLines = 2;
  numBlockBits = 8;
  cacheHitDelay = 4;
  memDelay = 100;
  flushAndLoadDelay = 200;
}

CMConfig::~CMConfig() {
}
