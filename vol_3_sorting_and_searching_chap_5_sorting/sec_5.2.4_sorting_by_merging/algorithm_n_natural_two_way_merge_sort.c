// algorithm_n_natural_two_way_merge_sort.c

// Algorithm N (Natural two-way merge sort)
// 5.2.4 Sorting by Merging
// The Art of Computer Programming, Donald Knuth

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// runs from opposite ends of the array are merged into workspace till
// forward and backward pointers meet
// then roles of array and workspace are switched, i.e. workspace becomes
// source and array becomes destination
// runs from opposite ends of workspace are merged into array till forward
// and backward pointers meet
// this alternating merging of runs is repeated
// the two ends of the destination array are called sides, switching sides
// means writing or transmitting the merged runs to the opposite end of the
// destination array
// on the other hand the two areas are the original array and the workspace
// array, switching areas means making the current destination array
// the source array and vice versa making the current source array the
// destination array

static void usage()
{
  puts("usage:algorithm_n_natural_two_way_merge_sort <in.dat >out.dat");
  puts("Implements Algorithm N (Natural two-way merge sort), 5.2.4 Sorting by Merging, The Art of Computer Programming Volume 3, Sorting and Searching by Donald Knuth");

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
  puts("algorithm_n_natural_two_way_merge_sort <data/algorithm_n_natural_two_way_merge_sort/in.0.le.dat | od -An -td8 -w8 -v");
}

// Sort takes array K of N elements beginning at K[1]
// Sort implements Algorithm N (Natural two-way merge sort)
// K is sorted in place
void Sort(const uint64_t N; int64_t K[2*N + 1], const uint64_t N)
{

  if(N == 0)
    return;

// N1 [Initialize] s <- 0
  uint8_t s = 0;

  for(;;) {

// forward pointer for source array
    uint64_t i;
// backward pointer for source array
    uint64_t j;
// forward pointer into destination array
    uint64_t k;
// backward pointer into destination array
    uint64_t l;

// N2 [Prepare for pass] i <- 1, j <- N, k <- N + 1, l <- 2N if s = 0
// s = 0 means source is original array, destination is workspace array
    if(s == 0) {
      i = 1;
      j = N;
      k = N + 1;
      l = 2 * N;
    }

// N2 [Prepare for pass] i <- N + 1, j <- 2N, k <- 1, l <- N if s = 1
// s = 1 means source is workspace array, destination is original array
    if(s == 1) {
      i = N + 1;
      j = 2 * N;
      k = 1;
      l = N;
    }

// N2 [Prepare for pass] d <- 1, f <- 1
// direction d is 1 forward from left, or -1 backward from right
// d works as increment for forward and backward pointers
    int8_t d = 1;
// finished f is made 0 if any merging occurs so sorting is done if f remains 1
    uint8_t f = 1;

// loop over one area till left and right pointers cross
// loop terminates with break in the middle
    for(;;) {

// N3 [Compare K_i:K_j] To N8 if K_i > K_j
// look for smaller key from right run
      if(K[i] > K[j]) {

// N8 [Transmit R_j] R_k <- R_j, k <- k + d
// emit smaller key to output area
        K[k] = K[j];
// update pointer into output area
// increment if output is on left side, decrement if output is on right side
        k += d;

// N9 [Stepdown?] j <- j - 1
        --j;

// N9 [Stepdown?] To N3 if K_(j + 1) <= K_j
// no stepdown in right run, continue comparing left and right runs
        if(K[j + 1] <= K[j]) {
          continue;
        }

// got stepdown on the right
// output complete left run
        do {

// N10 [Transmit R_i] R_k <- R_i, k <- k + d
          K[k] = K[i];
          k += d;

// N11 [Stepdown?] i <- i + 1
          ++i;

// N11 [Stepdown?] To N10 if K_(i - 1) <= K_i
        } while(K[i - 1] <= K[i]);

// N12 [Switch sides] f <- 0, d <- -d, k <-> l
// left run is done, continue to compare next run on the left with run on the right
// switch output side, swap current and previous output side pointers
        f = 0;
        d = -d;
        uint64_t tmp = k;
        k = l;
        l = tmp;

// N12 [Switch sides] To N3

      } else if(i == j) {

// N3 [Compare K_i:K_j] R_k <- R_i if i = j
// one area i.e. complete array has been processed since left and right run pointers crossed
        K[k] = K[i];

// N3 [Compare K_i:K_j] To N13 if i = j
        break;

      } else {

// N4 [Transmit R_i] R_k <- R_i, k <- k + d
// got smaller key from left run
        K[k] = K[i];
        k += d;

// N5 [Stepdown?] i <- i + 1
        ++i;

// N5 [Stepdown?] To N3 if K_(i - 1) <= K_i
// no stepdown in left run, continue comparing left and right runs
        if(K[i - 1] <= K[i])
          continue;

// got stepdown in left run
// output complete right run
        do {
// N6 [Transmit R_j]
          K[k] = K[j];
          k += d;

// N7 [Stepdown?] j <- j - 1
          --j;

// N7 [Stepdown?] To N6 if K_(j + 1) <= K_j
        } while(K[j + 1] <= K[j]);

// N12 [Switch sides] f <- 0, d <- -d, k <-> l
// right run is done, continue to compare next run on the right with run on the left
// switch output side, swap current and previous output side pointers
        f = 0;
        d = -d;
        uint64_t tmp = k;
        k = l;
        l = tmp;

// N12 [Switch sides] To N3
      }

    }

// N13 [Switch areas] Sorting is complete if f != 0
    if(f != 0) {
      break;
    }

// N13 [Switch areas] To N2, s <- 1 - s if f = 0
    s = 1 - s;
  }

// N13 [Switch areas]
  if(s == 1) {
// last output area was original array so it holds completely sorted output
    return;
  }

// N13 [Switch areas] (R_1,...,R_N) <- (R_(N + 1),...,R_2N)
// last output area was workspace array so copy it to original array
  for(uint64_t i = 1; i <= N; ++i) {
    K[i] = K[N + i];
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
// plus extra N entries for merge workspace
  int64_t R[2*N + 1];
  fread(&R[1], sizeof(*R), N, stdin);

  Sort(R, N);

// write number of values to follow
  fwrite(&N, sizeof N, 1, stdout);

// print sorted array as binary data
  fwrite(&R[1], sizeof(*R), N, stdout);

  return 0;
}

