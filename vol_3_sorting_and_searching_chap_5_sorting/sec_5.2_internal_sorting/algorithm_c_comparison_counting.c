// algorithm_c_comparison_counting.c

// Algorithm C (Comparison counting)
// 5.2 Internal sorting
// The Art of Computer Programming, Donald Knuth

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// usage: algorithm_c_comparison_counting <in.dat >out.dat
// reads 64-bit values to sort as binary data, outputs 64-bit counts as binary data
// first uint64_t is number of values to sort
// next that many int64_t is data to sort

// binary input data format
// uint64_t N
// int64_t[N] data

// examples:
// algorithm_c_comparison_counting <data/algorithm_c_comparison_counting/in.0.le.dat | od -An -td8 -w8 -v


// Sort implements Algorithm C
// Sort takes array K of N elements
// fills array COUNT with counts of elements less than the corresponding element of K, i.e. rank of element of K
void Sort(const int64_t K[], uint64_t COUNT[], const uint64_t N)
{
// C1 [Clear COUNTs] COUNT[i] <- 0 for 1 <= i <= N
  for(uint64_t i = 1; i <= N; ++i) {
    COUNT[i] = 0;
  }

// C2 [Loop on i] Perform C3 for i = N, N-1, ..., 2
  for(uint64_t i = N; i >= 2; --i) {

// C3 [Loop on j] Perform C4 for j = i-1, ..., 1
    for(uint64_t j = i - 1; j >= 1; --j) {

      if(K[i] < K[j]) {

// C4 [Compare K_i:K_j] COUNT[j] <- COUNT[j] + 1 if K_i < K_j
        ++COUNT[j];

      } else {

// C4 [Compare K_i:K_j] COUNT[i] <- COUNT[i] + 1 otherwise
        ++COUNT[i];
      }
    }
  }
}

#ifdef ALGORITHM_C_COMPARISON_COUNTING_BUILD_MAIN

int main(int argc, char* argv[])
{

// read 64-bit size of data array as binary data
  uint64_t N;
  fread(&N, sizeof N, 1, stdin);

// read array K as binary data
// allocate N+1 entries to use array indexing from 1 instead of 0
  int64_t K[N + 1];
  fread(&K[1], sizeof(*K), N, stdin);

// allocate COUNT array, extra entry again for 1-indexing
  uint64_t COUNT[N + 1];

// fill COUNT array
  Sort(K, COUNT, N);

// print COUNT array as binary data
  fwrite(&COUNT[1], sizeof(*COUNT), N, stdout);

  return 0;
}

#endif

