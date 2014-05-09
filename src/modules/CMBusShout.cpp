/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMBusShout.h"
#include "debug.h"
#include "CMAddr.h"

CMBusShout::CMBusShout() {
  isDone = false;
}

CMBusShout::~CMBusShout() {
  delete addr;
}

void CMBusShout::update(CMAddr *_addr,
                        shout_t _shoutType,
                        CMJob *_requestingJob) {
  if (_addr != NULL) {
    addr = _addr;
  }
  shoutType = _shoutType;
  if (_requestingJob != NULL) {
    requestingJob = _requestingJob;
  }
  isDone = false;
}

char CMBusShout::toChar() {
  if (shoutType == BusRd)
    return 'R';
  else if (shoutType == BusRdX)
    return 'X';
  else if (shoutType == BusWr)
    return 'W';
  else if (shoutType == BusUpg) {
    return 'U';
  } else {
    dassert(false, "invalid shout type!!!");
    return 'I';
  }
}

void CMBusShout::print() {
  if (shoutType == BusRd)
    dprintf("BusRd\n");
  else if (shoutType == BusRdX)
    dprintf("BusRdX\n");
  else if (shoutType == BusWr)
    dprintf("BusWr\n");
  else
    dprintf("BusUpg\n");
}
