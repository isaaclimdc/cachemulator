/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMBusShout.h"

CMBusShout::CMBusShout(CMAddr *_addr,
                       shout_t _shoutType,
                       CMJob *_requestingJob) {
  addr = _addr;
  shoutType = _shoutType;
  requestingJob = _requestingJob;
  isDone = false;
}

CMBusShout::~CMBusShout() {
}
