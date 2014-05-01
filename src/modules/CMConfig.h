/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

class CMConfig {
  private:

  public:
    CMConfig();
    ~CMConfig();

    int num_procs;
    int max_trace_line_length;
    int num_set_bits;
    int num_lines;
    int num_block_bits;
    int cache_hit_delay;
};
