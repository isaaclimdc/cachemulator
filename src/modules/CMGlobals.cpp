/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMGlobals.h"
#include <cstddef>
#include <sstream>
#include <fstream>

#define TMP_TRACE_PREFIX "traceFileProc"

CMConfig *CONFIG = NULL;
bool *BUSRequests = NULL;
std::ofstream *busShoutsFile = new std::ofstream();
std::ofstream *hitsMissesFile = new std::ofstream();


std::string MAKE_TMP_FILEPATH(size_t pid) {
  std::ostringstream ostr;
  ostr << TMP_TRACE_PREFIX << pid << ".tmp";
  return ostr.str();
}
