# Cachemulator: A Snooping-based Coherent Parallel Cache Emulator
**Yuyang Guo** (yuyangg) and **Isaac Lim** (idl)

## What is it?

Cachemulator is a coherent multi-processor cache emulator that uses a
**snooping**-based cache coherent protocol. We have 2 goals:

- Compare the performance of the MSI / MESI / MESIF protocols
- Provide cache usage information including bus traffic, hits/misses/evictions,
total ticks required, highly contended cache lines, false sharing, etc.

## A Sneak Peak

**UPDATE AS OF 5/7:**

- We have implemented all three protocols (**MSI**, **MESI**, and **MESIF**), and the
emulator as it stands passes all of our correctness tests.
- We are also successfully reporting some statistics about the cache usage of
a given memory trace, such as:
    - Amount of **bus traffic** measured by the number and types of bus requests.
    - Total number of **ticks** to run the trace to completion, where delays due to
    cache hits, memory accesses, and cache-to-cache transfers are all **configurable**.
  We show that MESIF substantially reduces the number of ticks (latency) over
  MESI, and MESI over MSI.
- Using `Pin` and a Pintool, we are able to generate a full memory trace of any
given binary, with each memory access annotated with its processor ID.
- Report **false sharing** and **highly-contended** cache lines.
- Because full memory traces of programs are extremely large, our more complicated pintool extracts the memory trace for only the most important parts of the program.