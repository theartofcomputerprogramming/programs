// algorithm_s_straight_insertion_sort.c

// Algorithm S (Straight insertion sort)
// 5.2.1 Sorting by Insertion
// The Art of Computer Programming, Donald Knuth

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void usage()
{
  puts("usage:algorithm_s_straight_insertion_sort <in.dat >out.dat");

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
  puts("algorithm_s_straight_insertion_sort <data/algorithm_s_straight_insertion_sort/in.0.le.dat | od -An -td8 -w8 -v");
}


// Sort takes array K_ of N elements beginning at K_[1]
// Sort implements Algorithm S (Straight insertion sort)
// K_ is sorted in place
void Sort(const uint64_t N, int64_t K_[N])
{

// S1 [Loop on j] Perform S2 through S5 for j = 2, 3, ..., N
  for(uint64_t j = 2; j <= N; ++j) {

    int64_t K;
    uint64_t i;

    for(

// S2 [Set up i, K, R] i <- j - 1, K <- K_j
      i = j - 1, K = K_[j];

// S3 [Compare K:K_i] To S3 if i > 0
      i > 0;

// S4 [Move R_i, decrease i] i <- i - 1
      --i
    ) {

// S3 [Compare K:K_i] To S5 if K >= K_i
      if(K >= K_[i])
        break;

// S4 [Move R_i, decrease i] R_{i+1} <- R_i
      K_[i + 1] = K_[i];

    }

// S5 [R into R_{i+1}] R_{i+1} <- R
    K_[i + 1] = K;

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


