// algorithm_m_merge_exchange.c

// Algorithm M (Merge exchange sort)
// Also solution to exercise 5.2.2.12
// 5.2.2 Sorting by Exchanging
// The Art of Computer Programming, Donald Knuth

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

static void usage()
{
  puts("usage:algorithm_m_merge_exchange <in.dat >out.dat");

  puts("reads 64-bit values as binary data to sort, outputs sorted 64-bit values as binary data");

  puts("first uint64_t is number of values to sort");
  puts("next that many int64_t is data to sort");

  puts("");
  puts("binary input data format");
  puts("uint64_t N");
  puts("int64_t[N] data");

  puts("");
  puts("binary output data format");
  puts("uint64_t N");
  puts("int64_t[N] sorted data");

  puts("");
  puts("examples:");
  puts("algorithm_m_merge_exchange <data/algorithm_m_merge_exchange/in.0.le.dat | od -An -td8 -w8 -v");
}

// Sort takes array K of N elements beginning at K[1]
// Sort implements Algorithm M (Merge exchange sort)
// K is sorted in place
void Sort(const uint64_t N, int64_t K[N + 1])
{

// algorithm requires N >= 2
  if(N < 2) {
    return;
  }

// M1 [Initialize p] t = ceil(lg N)
  uint64_t t = ceil(log2(N));

// loop on p over powers of 2 from 2^(t - 1) down to 1
  for(

// M1 [Initialize p] p <- 2^(t - 1)
    uint64_t p = pow(2.0, t - 1);

// M6 [Loop on p] To M2 if p > 0
    p > 0;

// M6 [Loop on p] p <- floor(p / 2)
    p /= 2
  ) {

// loop on q also on powers of 2 from 2^(t - 1) down to p
    for(

// d is step size for comparing pairs of keys
// M2 [Initialize q, r, d] q <- 2^(t - 1), r <- 0, d <- p
      uint64_t q = pow(2.0, t - 1), r = 0, d = p;

// loop runs while q != p after first iteration
// could be structured as do-while or unrolled into first pass and loop with guard
// chose to terminate with break below
      ;

// M5 [Loop on q] To M3 after d <- q - p, q <- q / 2, r <- p
      d = q - p, q /= 2, r = p
    ) {

// loop on i from start of array upto index that allows comparisons of pairs that are d-apart

// M3 [Loop on i] Perform M4 for 0 <= i < N - d and i & p = r
      for(uint64_t i = 0; i < N - d; ++i) {

// M3 [Loop on i] Perform M4 if i & p = r
        if((i & p) != r)
          continue;

// M4 [Compare/exchange R_(i + 1) : R_i(i + d +1)] Swap if K_(i + 1) > K_(i + d + 1)
        if(K[i + 1] <= K[i + d + 1])
          continue;

// M4 [Compare/exchange R_(i + 1) : R_i(i + d +1)] R_(i + 1) <-> R_i(i + d +1)
        uint64_t tmp = K[i + 1];
        K[i + 1] = K[i + d + 1];
        K[i + d + 1] = tmp;

      }

// M3 [Loop on i] To M5

// M5 [Loop on q] Continue if q != p
      if(q == p)
        break;

    }

  }

}

int main(int argc, char* argv[])
{

  if(argc > 1) {
    usage();
    exit(0);
  }

// read 64-bit size of data array as binary data
  uint64_t N;
  fread(&N, sizeof N, 1, stdin);

// read array R of records as binary data
// allocate N+1 entries to use array indexing from 1 instead of 0
  int64_t R[N + 1];
  fread(&R[1], sizeof(*R), N, stdin);

  Sort(N, R);

// write number of values to follow
  fwrite(&N, sizeof N, 1, stdout);

// print sorted array as binary data
  fwrite(&R[1], sizeof(*R), N, stdout);

  return 0;
}

