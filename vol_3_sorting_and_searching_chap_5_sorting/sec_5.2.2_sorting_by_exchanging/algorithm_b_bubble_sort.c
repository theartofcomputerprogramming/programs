// algorithm_b_bubble_sort.c

// Algorithm B (Bubble sort)
// 5.2.2 Sorting by Exchanging
// The Art of Computer Programming, Donald Knuth

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void usage()
{
  puts("usage:algorithm_b_bubble_sort <in.dat >out.dat");
  puts("Implements Algorithm B (Bubble sort), 5.2.2 Sorting by Exchanging, The Art of Computer Programming Volume 3, Sorting and Searching by Donald Knuth");

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
  puts("algorithm_b_bubble_sort <data/algorithm_b_bubble_sort/in.0.le.dat | od -An -td8 -w8 -v");
}

// Sort takes array K of N elements beginning at K[1]
// Sort implements Algorithm B (Bubble sort)
// K is sorted in place
void Sort(const uint64_t N, int64_t K[N + 1])
{

  for(

// B1 [Initialize BOUND] BOUND <- N
    uint64_t BOUND = N, t;

// loop while t != 0 checked at end of each iteration
    ;

// B4 [Any exchanges?] BOUND <- t, then to B2
    BOUND = t
  ) {

// B2 [Loop on j] t <- 0
    t = 0;

// B2 [Loop on j] Perform B3 for j = 1, 2, ..., BOUND - 1. Then to B4
    for(uint64_t j = 1; j <= BOUND - 1; ++j) {

// B3 [Compare/exchange R_j : R_(j+1)] Swap if K_j > K_(j+1)
      if(K[j] <= K[j + 1])
        continue;

// B3 [Compare/exchange R_j : R_(j+1)] R_j <-> R_(j+1)
      uint64_t tmp = K[j];
      K[j] = K[j + 1];
      K[j + 1] = tmp;

// B3 [Compare/exchange R_j : R_(j+1)] t <- j
      t = j;

    }

// B4 [Any exchanges?] Stop if t = 0
    if(t == 0)
      break;
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

