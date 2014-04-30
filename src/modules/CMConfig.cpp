/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMConfig.h"

CMConfig::CMConfig() {
  // TODO: parse a file to do all these settings
  num_procs = 4;
  max_trace_line_length = 20;
  num_set_bits = 8;
  num_lines = 2;
  num_block_bits = 8;
}
