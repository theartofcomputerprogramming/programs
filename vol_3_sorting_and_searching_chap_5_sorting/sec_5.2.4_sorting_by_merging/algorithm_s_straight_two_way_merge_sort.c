// algorithm_s_straight_two_way_merge_sort.c

// Algorithm S (Straight two-way merge sort)
// 5.2.4 Sorting by Merging
// The Art of Computer Programming, Donald Knuth

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// very similar to algorithm n natural two-way merge sort
// runs here are determined artificially using the fact that merging two
// runs of equal length results in a run double the length
// so runs are simply assumed to start with length 1 and doubled with
// every pass through the source array
// when run length crosses the size of the array we know we're done
// source and output arrays works exactly the same as algorithm n
// sides of the arrays also work the same way
// the difference is that sides are switched based on current run length
// instead of coming across a stepdown in the run

// this c implementation introduces two variables to simulate couple gotos
// with loop structures

static void usage()
{
  puts("usage:algorithm_s_straight_two_way_merge_sort <in.dat >out.dat");
  puts("Implements Algorithm S (Straight two-way merge sort), 5.2.4 Sorting by Merging, The Art of Computer Programming Volume 3, Sorting and Searching by Donald Knuth");

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
  puts("algorithm_s_straight_two_way_merge_sort <data/algorithm_s_straight_two_way_merge_sort/in.0.le.dat | od -An -td8 -w8 -v");
}

// Sort takes array K of N elements beginning at K[1]
// Sort implements Algorithm S (Straight two-way merge sort)
// K is sorted in place
void Sort(const uint64_t N; int64_t K[2*N + 1], const uint64_t N)
{

  if(N == 0)
    return;

// S1 [Initialize] s <- 0, p <- 1
// s is 0 for original array as source, 1 for workspace array as source
  uint8_t s = 0;

// p is power of 2 length of run to merge
  uint64_t p = 1;

// loop till sorting is done, determined when run length crosses array size
// loop terminates with break below
  for(;;) {

// forward pointer for source array
    uint64_t i;
// backward pointer for source array
    uint64_t j;
// forward pointer into output array
    uint64_t k;
// backward pointer into output array
    uint64_t l;


// S2 [Prepare for pass] i <- 1, j <- N, k <- N + 1, l <- 2N if s = 0
// source area is original array, output area is workspace
    if(s == 0) {
      i = 1;
      j = N;
      k = N;
      l = 2 * N + 1;
    }

// S2 [Prepare for pass] i <- N + 1, j <- 2N, k <- 1, l <- N if s = 1
// source area is workspace, output area is original array
    if(s == 1) {
      i = N + 1;
      j = 2 * N;
      k = 0;
      l = N + 1;
    }

// S2 [Prepare for pass] d <- 1, f <- 1
// direction d is 1 forward from left, or -1 backward from right
    int8_t d = 1;

// counter for number of keys remaining to merge for current run length from left side
    uint64_t q = p;
// counter for number of keys remaining to merge for current run length from right side
    uint64_t r = p;

// loop over one area till left and right pointers cross
    for(;;) {

// need this to simulate goto S12 from S7 in algorithm
      bool goto_s12_switch_sides;

// S3 [Compare K_i:K_j] To S8 if K_i > K_j
      if(K[i] > K[j]) {

// S8 [Transmit R_j] k <- k + d, R_k <- R_j
        k += d;
        K[k] = K[j];

// S9 [End of run?] j <- j - 1, r <- r - 1
        --j;
        --r;
// S9 [End of run?] To S3 if r > 0
        if(r > 0)
          continue;

      } else {

// S4 [Transmit R_i] k <- k + d, R_k <- R_i
// emit smaller key from left run to output area
        k += d;
        K[k] = K[i];

// S5 [End of run?] i <- i + 1, q <- q - 1
        ++i;
        --q;

// S5 [End of run?] To S3 if q > 0
// run on left is not completely merged, continue comparing keys from left and right
        if(q > 0)
          continue;

// run from left is completely merged, equivalent of stepdown in algorithm n
// copy all remaining keys from run on right side

// S6 [Transmit R_j] To S13 if k = l, k <- k + d
// loop over run on right side
        for(k += d, goto_s12_switch_sides = false; k != l; k += d) {

// S6 [Transmit R_j] R_k <- R_j
          K[k] = K[j];

// S7 [End of run?] j <- j - 1, r <- r - 1
// decrement source pointer from right side
          --j;
// decrement counter of remaining keys in run
          --r;

// S7 [End of run?] To S6 if r > 0
          if(r == 0) {
// set flag to enter correct code block for switching sides
            goto_s12_switch_sides = true;
            break;
          }

        }

// S6 [Transmit R_j] To S13 if k = l
        if(k == l)
          break;

      }

// extra copy of S12 so it can run directly after S7 and as part of S10-S12 loop below
      if(goto_s12_switch_sides) {
        goto_s12_switch_sides = false;
// S12 [Switch sides] q <- p, r <- p, d <- -d, k <-> l
        q = p;
        r = p;
        d = -d;
        uint64_t tmp = k;
        k = l;
        l = tmp;

// S12 [Switch sides] To S10 if j - i < p, to S3 otherwise
// test is expressed bit differently for unsigned values
        if(j >= p + i) {
          continue;
        }
      }

// S10 [Transmit R_i] To S13 if k = l, k <- k + d
      for(k += d; k != l; k += d) {

// S10 [Transmit R_i] R_k <- R_i if k != l
        K[k] = K[i];

// S11 [End of run?] i <- i + 1, q <- q - 1
        ++i;
        --q;
// S11 [End of run?] To S10 if q > 0
        if(q > 0) {
          continue;
        }

// S12 [Switch sides] q <- p, r <- p, d <- -d, k <-> l
        q = p;
        r = p;
        d = -d;
        uint64_t tmp = k;
        k = l;
        l = tmp;

// S12 [Switch sides] To S10 if j - i < p, to S3 otherwise
// test is expressed bit differently for unsigned values
        if(j >= p + i) {
          break;
        }

      }

// S10 [Transmit R_i] To S13 if k = l
// output area pointers crossed, time to switch areas
      if(k == l)
        break;

    }

// S13 [Switch areas] p <- p + p
// double run length for next pass
    p += p;

// S13 [Switch areas] Sorting is complete
    if(p >= N)
      break;

// S13 [Switch areas] To S2, s <- 1 - s if p < N
    s = 1 - s;

  }

// S13 [Switch areas] Sorting is complete
// simply return if original array was the last output area
  if(s == 1)
    return;

// S13 [Switch areas] Sorting is complete, (R_1,...,R_N) <- (R_(N + 1),...,R_2N) if s = 0
// copy workspace array into original array if workspace was last output area
  for(uint64_t i = 1; i <= N; ++i) {
    K[i] = K[N + i];
  }

}

int main(int argc, char* argv[])
{

  (void)argv;

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

