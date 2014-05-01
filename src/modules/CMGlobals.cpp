/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMGlobals.h"
#include <malloc.h>

CMConfig *CONFIG = new CMConfig();
bool *BUSRequests = (bool*)calloc(CONFIG->num_procs, sizeof(bool));
bool *BUSGranted = (bool*)calloc(CONFIG->num_procs, sizeof(bool));
