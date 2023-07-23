// algorithm_s_straight_selection_sort.c

// Algorithm S (Straight selection sort)
// 5.2.3 Sorting by Selection
// The Art of Computer Programming, Donald Knuth

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

static void usage()
{
  puts("usage:algorithm_s_straight_selection_sort <in.dat >out.dat");

  puts("reads 64-bit values as binary data to sort, outputs sorted 64-bit values as binary data");

  puts("first uint64_t is number of values to sort");
  puts("next that many uint64_t is data to sort");

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
  puts("algorithm_s_straight_selection_sort <data/algorithm_s_straight_selection_sort/in.0.le.dat | od -An -td8 -w8 -v");
}

// Sort takes array K of N keys beginning at K[1]
// Sort implements Algorithm R (Radix exchange sort)
// K is sorted in place
void Sort(const uint64_t N, int64_t K[N + 1])
{

// S1 [Loop on j] Perform S2 and S3 for j = N, N - 1, ..., 2
  for(uint64_t j = N; j >= 2; --j) {

// S2 [Find max(K_1,...,K_j)] Find maximal K_i among K_j, K_(j-1),...,K_1 where i is as large as possible
    uint64_t i = j;

    for(uint64_t l = j - 1; l >= 1; --l) {
      if(K[l] > K[i]) {
        i = l;
      }
    }

// S3 [Exchange with R_j] R_i <-> R_j
    int64_t tmp = K[j];
    K[j] = K[i];
    K[i] = tmp;
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
  int64_t R[N + 1];

  fread(&R[1], sizeof(*R), N, stdin);

  Sort(N, R);

// write number of values to follow
  fwrite(&N, sizeof N, 1, stdout);

// print sorted array as binary data
  fwrite(&R[1], sizeof(*R), N, stdout);

  return 0;
}



