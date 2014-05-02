/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMBusCtrlr.h"
#include "CMGlobals.h"
#include "debug.h"

CMBusCtrlr::CMBusCtrlr() {
  roundRobin = 0;
}

CMBusCtrlr::~CMBusCtrlr() {
}

// Use round robin arbitration to grant access. Returns pid
// of the 'winning' processor.
size_t CMBusCtrlr::tick() {
  size_t arb = 0;

  int P = CONFIG->numProcs;

  for (int i=roundRobin; i<roundRobin + P; ++i) {
    arb = i % P;
    if (BUSRequests[arb]) {
      break;
    }
  }

  roundRobin = (roundRobin + 1) % P;
  memset(BUSRequests, 0, sizeof(bool) * P);

  return arb;
}
