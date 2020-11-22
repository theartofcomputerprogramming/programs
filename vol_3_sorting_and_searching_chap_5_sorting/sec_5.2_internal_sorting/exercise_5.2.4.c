// exercise_5.2.4.c

// Exercise 5.2.4
// 5.2 Internal sorting
// The Art of Computer Programming, Donald Knuth

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "algorithm_c_comparison_counting.h"

// usage: exercise_5.2.4 <in.dat >out.dat
// reads 64-bit values to sort as binary data, outputs sorted 64-bit values as binary data
// first uint64_t is number of values to sort
// next that many int64_t is data to sort

// binary input data format
// uint64_t N
// int64_t[N] data

// examples:
// exercise_5.2.4 <data/exercise_5.2.4/in.0.le.dat | od -An -td8 -w8 -v


// Finish is a solution to exercise 5.2.4
// Finish takes arrays K of keys and COUNT of rankings of the keys and fills the array OUTPUT with the sorted keys from K
// all arrays are 1-indexed
void Finish(const int64_t K[], const uint64_t COUNT[], const uint64_t N, int64_t OUTPUT[N])
{
  for(uint64_t i = 1; i <= N; ++i) {
// add 1 to rank due to 1-indexing
    uint64_t sortedIndex = COUNT[i] + 1;
    OUTPUT[sortedIndex] = K[i];
  }
}

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

// use COUNT array to fill sorted array OUTPUT
  int64_t OUTPUT[N + 1];
  Finish(K, COUNT, N, OUTPUT);

// print OUTPUT array as binary data
  fwrite(&OUTPUT[1], sizeof(*OUTPUT), N, stdout);

  return 0;
}

