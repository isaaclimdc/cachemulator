/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMGlobals.h"

CMConfig *CONFIG = new CMConfig();
bool *BUSRequests = new bool[CONFIG->num_procs];
