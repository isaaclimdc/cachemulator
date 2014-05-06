/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMGlobals.h"
#include <cstddef>
#include <sstream>

#define TMP_TRACE_PREFIX "traceFileProc"

CMConfig *CONFIG = NULL;
bool *BUSRequests = NULL;

std::string MAKE_TMP_FILEPATH(size_t pid) {
  std::ostringstream ostr;
  ostr << TMP_TRACE_PREFIX << pid << ".tmp";
  return ostr.str();
}
