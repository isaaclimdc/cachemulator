/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <stdarg.h>
#include <assert.h>

#include "cache.h"
#include "CMCache.h"
#include "modules/CMSet.h"
#include "modules/CMAddr.h"
#include "modules/CMLine.h"

#define MAX_TRACE_LINE_LENGTH 20

static CMCache *cache;

int main(int argc, char **argv) {
  int s = 0;
  int E = 0;
  int b = 0;
  char *filePath;

  char c;
  while ((c = getopt(argc, argv, "s:E:b:t:v")) != -1) {
    switch (c) {
      case 's':
        s = atoi(optarg);
        break;
      case 'E':
        E = atoi(optarg);
        break;
      case 'b':
        b = atoi(optarg);
        break;
      case 't':
        filePath = optarg;
        break;
      default:
        exit(1);
    }
  }

  // Allocate and initialize the main cache
  cache = new CMCache(s, E);

  int hits = 0;
  int misses = 0;
  int evicts = 0;

  FILE *traceFile = fopen(filePath, "r");

  if (traceFile == NULL) {
    perror("Error opening file.");
  }

  char traceLine[MAX_TRACE_LINE_LENGTH];

  while (fgets(traceLine, MAX_TRACE_LINE_LENGTH, traceFile) != NULL) {
    if (traceLine[0] == ' ') {
      char op;
      long long unsigned raw_addr;
      int size;

      sscanf(traceLine+1, "%c %llx,%d", &op, &raw_addr, &size);

      CMAddr *addr = new CMAddr(raw_addr, s, b); //TODO: Use a stack var here?

      // Separate a Modify instruction into a Load then a Store.
      if (op == 'M') {
        calculate_stats(addr, E, 'L', size, hits, misses, evicts);
        op = 'S';
      }

      calculate_stats(addr, E, op, size, hits, misses, evicts);

      delete addr;
    }
  }

  print_summary(hits, misses, evicts);

  // When all is done, clean up remaining memory
  fclose(traceFile);
  delete cache;

  return 0;
}

void calculate_stats(CMAddr *addr, int E, char op, int size,
                     int &hits, int &misses, int &evicts)
{
  // The set matched by the setIndex of the address
  CMSet *target_set = cache->sets.at(addr->set_index);

  // Iterate through the E lines in the set to search for a hit
  std::vector<CMLine *> lines = target_set->lines;
  std::vector<CMLine *>::iterator it;
  for (it = lines.begin(); it != lines.end(); ++it) {
    CMLine *line = *it;

    // It's a hit!
    if (line->valid && line->tag == addr->tag) {
      hits++;
      adjust_ages(target_set, line, E);

      // Only write on hit if a Store op
      if (op == 'S') {
        write_to_line(target_set, E, line, addr);
      }

      break;
    }
  }

  // Can't find the tag in the set, so it's a miss!
  if (hits == 0) {
    misses++;

    // A boolean to indicate to use LRU if no empty lines are found
    bool replacementFound = false;

    // Search for an empty (not valid) line to store
    std::vector<CMLine *>::iterator it;
    for (it = lines.begin(); it != lines.end(); ++it) {
      CMLine *line = *it;

      if (!(line->valid)) {
        write_to_line(target_set, E, line, addr);
        replacementFound = true;
        break;
      }
    }

    // No empty line, so use LRU replacement policy
    if (!replacementFound) {
      evicts++;

      // Write to the least recent line in targetSet, according to LRU
      CMLine *line = get_least_recent_line(target_set, E);
      write_to_line(target_set, E, line, addr);
    }
  }
}

/* Performs the write tasks of address A to line L. Then adjusts
 * the ages of every line in the given set accordingly.
 */
void write_to_line(CMSet *target_set, int E, CMLine *line, CMAddr *addr) {
  line->valid = 1;
  line->tag = addr->tag;

  adjust_ages(target_set, line, E);
}

/* Adjust the ages of each line in the given set. Increments
 * the age of every line in the set, and resets the age of resetLine
 * to 1. This is the most recently updated line.
 */
void adjust_ages(CMSet *target_set, CMLine *reset_line, int E) {
  std::vector<CMLine *> lines = target_set->lines;
  std::vector<CMLine *>::iterator it;
  for (it = lines.begin(); it != lines.end(); ++it) {
    CMLine *L = *it;
    if (L == reset_line) { // TODO: Can't compare CMLine objects like this right?
      L->age = 1;
    }
    else {
      L->age++;
    }
  }
}

/* Linear search through all lines in the given set for the index
 * of the 'oldest' line (max age). This is the least recent line
 * in the set. Returns its index.
 */
CMLine *get_least_recent_line(CMSet *target_set, int E) {
  int max_age = 0;
  CMLine *least_recent_line;

  std::vector<CMLine *> lines = target_set->lines;
  std::vector<CMLine *>::iterator it;
  for (it = lines.begin(); it != lines.end(); ++it) {
    CMLine *line = *it;
    int age = line->age;
    if (age > max_age) {
      max_age = age;
      least_recent_line = line;
    }
  }

  return least_recent_line;
}

/*************************************/
/************** Helpers **************/
/*************************************/

void print_summary(int hits, int misses, int evictions) {
  printf("Hits:%d Misses:%d Evictions:%d\n", hits, misses, evictions);
  FILE *output_fp = fopen(".csim_results", "w");
  assert(output_fp);
  fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
  fclose(output_fp);
}
