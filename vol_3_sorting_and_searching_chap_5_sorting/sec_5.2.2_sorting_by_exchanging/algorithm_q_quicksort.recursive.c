// algorithm_q_quicksort.c

// Algorithm Q (Quicksort)
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
  puts("usage:algorithm_q_quicksort <in.dat >out.dat");

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
  puts("algorithm_q_quicksort <data/algorithm_q_quicksort/in.0.le.dat | od -An -td8 -w8 -v");
}

// straight_insertion_sort
// implements Algorithm 5.2.1S (Straight insertion sort)
// with enhancements from exercises 5.2.1.10 and 5.2.1.33
static void straight_insertion_sort(const uint64_t N, int64_t K_[N+1])
{

// Q9 [Straight insertion sort]

// For j = 2, 3,...,N
  for(uint64_t j = 2; j <= N; ++j) {

// Loop on i if K_(j - 1) > K_j
    if(K_[j - 1] <= K_[j])
      continue;

// Loop on i
// enhancement from exercise 5.2.1.33
// use the fact that K_[0] = INT64_MIN as sentinel to eliminate bounds check on i

// K <- K_j, R <- R_j
    int64_t K = K_[j];
    uint64_t i;

// i <- j - 1
// R_(i + 1) <- R_i, i <- i - 1 until K_i <= K
    for(i = j - 1; K < K_[i]; --i) {
      K_[i + 1] = K_[i];
    }

// R_(i + 1) <- R
    K_[i + 1] = K;

  }

}

// recursive partition exchange
static void Q2Stage(uint64_t l, uint64_t r, const uint64_t N, int64_t K_[N + 2], const uint64_t M)
{

// Q8 [Take off stack] To Q2, (l', r'0 <= stack if stack nonempty, l <- l', r <- r'
// step Q8 same as step Q2 due to recursion

// runs while partitions are longer than M
// terminates by break below
  for(;;) {

// Q2 [Begin new stage] i <- l, j <- r + 1, K <- K_l
// i is pointer moving forward through left partition
// j is pointer moving backward through right partition
    uint64_t i = l;
    uint64_t j = r + 1;

// select left partition boundary key for pivot key K
    int64_t K = K_[l];

// find pair of keys to exchange from the two partitions
// runs while i < j, terminates by break below
    for(;;) {

// Q3 [Compare K_i : K] i <- i + 1, repeat while K_i < K
// forward loop over left partition till a key bigger than pivot is found
      for(++i; K_[i] < K; ++i);

// Q4 [Compare K : K_j] j <- j - 1, repeat while K < K_j
// reverse loop over right partition till a key smaller than pivot is found
      for(--j; K < K_[j]; --j);

// Q5 [Test i : j] To Q7, R_l <-> R_j if j <= i
// could not find pair of keys to swap because right pointer crossed left pointer
      if(j <= i) {
        int64_t tmp = K_[l];
        K_[l] = K_[j];
        K_[j] = tmp;
        break;
      }

// Q6 [Exchange] To Q3, R_i <-> R_j
// found one key from each partition to swap
// swap and continue scanning both partitions
      int64_t tmp = K_[i];
      K_[i] = K_[j];
      K_[j] = tmp;

    }

// select longer of left and right partitions for recursion
// adjust and continue working on other partition if it's not shorter than threshold for insertion sort

// Q7 [Put on stack] To Q2, (j + 1, r) => stack, r <- j - 1 if r - j >= j - l > M
    if(r - j >= j - l && j - l > M) {
// right partition is longer, recursively partition it
// step Q8 happens inside recursive call to Q2Stage
      Q2Stage(j + 1, r, N, K_, M);
// move right boundary down and continue working on current partition
      r = j - 1;
      continue;
    }

// Q7 [Put on stack] To Q2, (l, j - 1) => stack, l <- j + 1 if j - l > r - j > M
    if(j - l > r - j && r - j > M) {
// left partition is longer, recursively partition it
// step Q8 happens inside recursive call to Q2Stage
      Q2Stage(l, j - 1, N, K_, M);
// move left boundary up and continue working on current partition
      l = j + 1;
      continue;
    }

// one partition is shorter than threshold length for insertion sort
// only continue to work on the other partition

// Q7 [Put on stack] To Q2, l <- j + 1 if r - j > M >= j - l
// left partition is shorter than threshold length
    if(r - j > M && M >= j - l) {
// move left boundary up and continue working on right partition
      l = j + 1;
      continue;
    }

// Q7 [Put on stack] To Q2, r <- j - 1 if j - l > M >= r - j
// right partition is shorter than threshold length
    if(j - l > M && M >= r - j) {
// move right boundary down and continue working on left partition
      r = j - 1;
      continue;
    }

// both partitions are shorter than threshold length for insertion sort
    break;
  }

}

// Sort takes array K of N+2 elements with keys in K[1..N]
// K[0] must be -Inf, K[N + 1] must be +Inf
// Sort implements Algorithm Q (Quicksort)
// K is sorted in place
void Sort(const uint64_t N, int64_t K[N + 2])
{

// threshold length to switch to insertion sort
  const uint64_t M = 12;

// Q1 [Initialize] To Q9 if N <= M
  if(N <= M) {
// Q9 [Straight insertion sort]
    straight_insertion_sort(N, K);
    return;
  }

// Q1 [Initialize] l <- 1, r <- N
  const uint64_t l = 1;
  const uint64_t r = N;

// Q2 [Begin new stage]
  Q2Stage(l, r, N, K, M);

// array has now been partitioned around various pivots

// Q9 [Straight insertion sort]
// sort entire array with insertion sort
  straight_insertion_sort(N, K);

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
// allocate N+2 entries for special first and last values following note a) of Algorithm Q (Quicksort)
  int64_t R[N + 2];

  R[0] = INT64_MIN;
  R[N + 1] = INT64_MAX;

  fread(&R[1], sizeof(*R), N, stdin);

  Sort(N, R);

// write number of values to follow
  fwrite(&N, sizeof N, 1, stdout);

// print sorted array as binary data
  fwrite(&R[1], sizeof(*R), N, stdout);

  return 0;
}

