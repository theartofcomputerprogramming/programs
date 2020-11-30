// algorithm_d_shellsort.c

// Algorithm D (Shellsort)
// 5.2.1 Sorting by Insertion
// The Art of Computer Programming, Donald Knuth

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void usage()
{
  puts("usage:algorithm_d_shellsort <in.dat >out.dat");

  puts("reads 64-bit values as binary data to sort, outputs sorted 64-bit values as binary data");

  puts("first uint64_t is number of values to sort");
  puts("next that many int64_t is data to sort");
  puts("then uint64_t is number of increments to use");
  puts("next that many uint64_t are values of increment");
  puts("first increment value must be 1");

  puts("");
  puts("binary input data format");
  puts("uint64_t N");
  puts("int64_t[N] data");
  puts("uint64_t t");
  puts("uint64_t[t] increments");

  puts("");
  puts("binary output data format");
  puts("uint64_t N");
  puts("int64_t[N] sorted data");

  puts("");
  puts("examples:");
  puts("algorithm_d_shellsort <data/algorithm_d_shellsort/in.0.le.dat | od -An -td8 -w8 -v");
}


// Sort takes array K_ of N elements beginning at K_[1]
// Sort implements Algorithm D (Shellsort)
// K_ is sorted in place
void Sort(const uint64_t N, int64_t K_[N], const uint64_t t, uint64_t H[t])
{

  if(t == 0) {
    return;
  }

// D1 [Loop on s] Perform D2 for s = t - 1, t - 2,..., 0
  for(int64_t s = t - 1; s >= 0; --s) {

// D2 [Loop on j] h <- h_s, perform D3 through D6 for h < j <= N
    for(uint64_t h = H[s], j = h + 1; j <= N; ++j) {

      int64_t K;
      int64_t i;

      for(

// D3 [Set up i, K, R] i <- j - 1, K <- K_j
        i = j - h, K = K_[j];

// D5 [Move R_i, decrease i] To D4 if i > 0
        i > 0;

// D5 [Move R_i, decrease i] i <- i - h
        i -= h
      ) {

// D4 [Compare K:K_i] To D6 if K >= K_i
        if(K >= K_[i])
          break;

// D5 [Move R_i, decrease i] R_{i+h} <- R_i
        K_[i + h] = K_[i];

      }

// D6 [R into R_{i+h}] R_{i+h} <- R
      K_[i + h] = K;

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

// read 64-bit size of increments array as binary data
  uint64_t t;
  fread(&t, sizeof t, 1, stdin);

// read array of 64-bit increments as binary data
  uint64_t H[t];
  fread(H, sizeof(*H), t, stdin);

  if(H[0] != 1) {
    usage();
    exit(1);
  }

  Sort(N, R, t, H);

// write number of values to follow
  fwrite(&N, sizeof N, 1, stdout);

// print sorted array as binary data
  fwrite(&R[1], sizeof(*R), N, stdout);

  return 0;
}


