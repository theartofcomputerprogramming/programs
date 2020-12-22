// algorithm_r_radix_exchange_sort.recursive.c


// Algorithm R (Radix exchange sort)
// 5.2.2 Sorting by Exchanging
// The Art of Computer Programming, Donald Knuth
// uses recursion instead of explicit stack in algorithm

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

static void usage()
{
  puts("usage:algorithm_r_radix_exchange_sort.recursive <in.dat >out.dat");

  puts("reads 64-bit values as binary data to sort, outputs sorted 64-bit values as binary data");

  puts("first uint64_t is max number of bits needed for values");
  puts("second uint64_t is number of values to sort");
  puts("next that many int64_t is data to sort");

  puts("");
  puts("binary input data format");
  puts("uint64_t m");
  puts("uint64_t N");
  puts("int64_t[N] data");

  puts("");
  puts("binary output data format");
  puts("uint64_t N");
  puts("int64_t[N] sorted data");

  puts("");
  puts("examples:");
  puts("algorithm_r_radix_exchange_sort.recursive <data/algorithm_r_radix_exchange_sort.recursive/in.0.le.dat | od -An -td8 -w8 -v");
}

// Recursive part of Algorithm R (Radix exchange sort)
// replaces the stack used in the algorithm
// recursion is a kind of left recursion
// left partition is passed to recursive call
// caller then waits for recursion to complete to continue working on right partition
// callee instance returns the right boundary for caller instance to use as left boundary for the partition it will continue to work on
// unlike the stack used in the original algorithm to defer processing of
// right partitions, this version has to descend into recursive calls
// right away so the call stack holds left partitions being worked on
// and right partitions are deferred till recursion returns which is
// the equivalent of a stack being popped

static uint64_t R2Stage(uint64_t l, uint64_t r, uint64_t b, const uint64_t N, uint64_t K[N + 1])
{

// R2 [Begin new stage] To R10 if l = r
  for(; l != r;) {

    uint64_t i = l;
    uint64_t j = r;

    bool found_swap_pair = false;
// R4 [Increase i]
    for(
      bool R3_first_time = true;

// R4 [Increase i] To R3 if i <= j, to R8 otherwise
      R3_first_time || i <= j;
// R4 [Increase i] i <- i + 1
      ++i) {

      if(R3_first_time)
        R3_first_time = false;

// R3 [Inspect K_i for 1] To R6 if bit b of K_i is 1
      if((K[i] & b) == 0) {
        continue;
      }

      for(--j; i <= j; --j) {

// R5 [Inspect K_(j + 1) for 0] To R7 if bit b of K_(j + 1) is 0
        if((K[j + 1] & b) == 0) {
          found_swap_pair = true;
          break;
        }
      }

// R6 [Decrease j] To R8 if i > j
      if(!found_swap_pair) {
        break;
      }

// R7 [Exchange R_i, R_(j + 1)]
      uint64_t tmp = K[i];
      K[i] = K[j + 1];
      K[j + 1] = tmp;

      found_swap_pair = false;

    }

// one partitioning stage has completed

// R8 [Test special cases]
    b >>= 1;

// R8 [Test special cases] To R10 if b > m where m is number of bits in keys
    if(b == 0) {
      break;
    }

// R8 [Test special cases] To R2 if j < l or j = r
    if(j < l || j == r) {
      continue;
    }

    if(j == l) {
      ++l;
      continue;
    }

// R9 [Put on stack] (r, b) => stack, to R2 with r <- j
    uint64_t result = R2Stage(l, j, b, N, K);

// R10 [Take off stack] To R2, l <- r + 1, (r, b) <= stack
    l = result + 1;

  }

// R10 [Take off stack]
  return r;

}

// Sort takes array K of N unsigned keys beginning at K[1]
// Sort implements Algorithm R (Radix exchange sort)
// K is sorted in place
// m is max number of bits needed for range of keys
void Sort(const uint64_t N, uint64_t K[N + 1], const uint64_t m)
{

// R1 [initialize]
  const uint64_t l = 1;
  const uint64_t r = N;
  const uint64_t b = 1ul << (m - 1);

// R2 [Begin new stage]
  R2Stage(l, r, b, N, K);

}

int main(int argc, char* argv[])
{

  if(argc > 1) {
    usage();
    exit(0);
  }

// read 64-bit max number of bits as binary data
  uint64_t m;
  fread(&m, sizeof m, 1, stdin);

// read 64-bit size of data array as binary data
  uint64_t N;
  fread(&N, sizeof N, 1, stdin);

// read array R of records as binary data
  uint64_t R[N + 1];

  fread(&R[1], sizeof(*R), N, stdin);

  Sort(N, R, m);

// write number of values to follow
  fwrite(&N, sizeof N, 1, stdout);

// print sorted array as binary data
  fwrite(&R[1], sizeof(*R), N, stdout);

  return 0;
}



