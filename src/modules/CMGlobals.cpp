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
std::ofstream *busTrafficFile = new std::ofstream();

int totalInvalidations = 0;
int totalHits = 0;
int totalMisses = 0;
int totalEvictions = 0;
int totalInstructions = 0;
int totalBusRd = 0;
int totalBusRdX = 0;
int totalBusWr = 0;
int totalBusUpg = 0;
int totalC2C = 0;

std::string MAKE_TMP_FILEPATH(size_t pid) {
  std::ostringstream ostr;
  ostr << TMP_TRACE_PREFIX << pid << ".tmp";
  return ostr.str();
}
