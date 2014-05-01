/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMBusCtrlr.h"
#include "CMGlobals.h"
#include <string.h>
#include "debug.h"

CMBusCtrlr::CMBusCtrlr() {
  round_robin = 0;
}

CMBusCtrlr::~CMBusCtrlr() {
}

int CMBusCtrlr::tick() {
  // use round robin arbitration

  int arb = 0;
  for (int i=round_robin; i<round_robin + CONFIG->num_procs; ++i) {
    arb = i % CONFIG->num_procs;
    if (BUSRequests[arb]) {
      break;
    }
  }
  round_robin = (round_robin + 1) % CONFIG->num_procs;

  memset(BUSRequests, 0, sizeof(bool) * CONFIG->num_procs);

  return arb;
}
