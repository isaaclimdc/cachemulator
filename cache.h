/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

// Forward-declarations
class CMAddr;
class CMSet;
class CMLine;

void calculate_stats(CMAddr *addr, int E, char op, int size,
                     int &hits, int &misses, int &evicts);
void write_to_line(CMSet *target_set, int E, CMLine *L, CMAddr *A);
void adjust_ages(CMSet *target_set, CMLine *reset_line, int E);
CMLine *get_least_recent_line(CMSet *target_set, int E);
void print_summary(int hits, int misses, int evictions);
