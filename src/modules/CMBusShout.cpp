/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMBusShout.h"

CMBusShout::CMBusShout(CMAddr *addr, shout_t shoutType, CMJob *requestingJob) {
  addr = addr;
  shoutType = shoutType;
  requestingJob = requestingJob;
}

CMBusShout::~CMBusShout() {
}
