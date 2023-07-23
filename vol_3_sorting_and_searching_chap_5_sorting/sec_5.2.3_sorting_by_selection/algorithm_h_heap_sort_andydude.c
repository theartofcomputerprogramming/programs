// SPDX-FileCopyrightText: © 2023 Andrew Robbins
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define LIST_SWAP(X, i, j, t) \
  if (i != j) {		      \
    t = X[i];                 \
    X[i] = X[j];              \
    X[j] = t;                 \
  }


static void usage()
{
  puts("usage: algorithm_h_heap_sort < in.dat > out.dat");
}

/*
 * Williams INHEAP(X, N, K),
 * Williams SWAPHEAP(X, N, K, R)
 * Knuth Algorithm H3U (Sift up)
 *
 * **Program 5.2.3H3U** (*Sift up*)
 *
 * (Omitted because we are focusing on C.)
 *
 * **Algorithm 5.2.3H3U** (*Sift up*)
 *
 * **H3.** [Prepare for siftup.] Set `j <- l`. (At this point
 *     we have `K_floor(k/2) >= K_k` for `l < floor(k/2) < k <= r`;
 *     and record `R_k` is in its final position for `r < k <= N`.
 *     Steps H3-H8 are called the *siftup algorithm*; their effect
 *     is equivalent to setting `R_l <- R` and then rearranging
 *     `R_l, ..., R_r` so that condition (6) holds for
 *     `l = floor(k/2)`.)
 * **H4.** [Advance downward.] Set `i <- j` and `j <- 2j`.
 *     (In the following steps we have `i = floor(j/2)`.)
 *     If `j < r`, go right on to step H5; if `j = r`,
 *     go to step H6; and if `j > r`, go to H8.
 * **H5.** [Find larger child.] If `K_j < K_j+1`, then set `j <- j + 1`.
 * **H6.** [Larger than K?] If `K >= K_j`, then go to step H8.
 * **H7.** [Move it up.] Set `R_i <- R_j`, and go back to step H4.
 * **H8.** [Store R.] Set `R_i <- R`.
 *    (This terminates the siftup algorithm initiated in step H3.)
 *    Return to step H2.
 */
void
subroutine_5_2_3_h3u_sift_up(
	int64_t *X, uint64_t l,
	uint64_t k, uint64_t r)
{
  int64_t t;  // Temp for swap
  uint64_t i; // RosettaCode root
  uint64_t j; // RosettaCode child

  // **H3.** [Prepare for sift up.]
  i = l;

  while (true) {
    // **H4.** [Advance downward.]
    j = 2*i;

    if ((j < r) && (X[j] < X[j + 1])) {
      // **H5.** [Find larger child.]
      ++j;
    }

    if ((j > r) || !(X[i] < X[j])) {
      // **H8.** [Store the record.]
      LIST_SWAP(X, l, k, t);
      break;
    }

    if (X[i] < X[j]) {
      // **H6.** [Larger than the key?.]
      // **H7.** [Move it up.]
      LIST_SWAP(X, i, j, t);
      i = j;
    }
  }
}

/*
 * Knuth Algorithm H (Heap sort)
 * Williams Algorithm 232 HEAPSORT()
 *
 * **Program 5.2.3H** (*Heap sort*)
 *
 * (Omitted because we are focusing on C.)
 *
 * **Algorithm 5.2.3H** (*Heap sort*)
 *
 * Records `R_1, ..., R_N` are rearranged in place; after
 * sorting is complete, their keys will be in order,
 * `K_1 <= ... <= K_N`. First we rearrange the file so that
 * it forms a heap, then we repeatedly remove the top of the
 * heap and transfer it to its proper final position.
 * Assume that `N >= 2`.
 *
 * **H1.** [Initialize.] Set l <- floor(N/2) + 1, r <- N.
 * **H2.** [Decrease l or r.] If l > 1, set l <- l - 1, R <- R_l, K <- K_l.
 *     Otherwise set R <- R_r, K <- K_r, R_r <- R_1, and r <- r - 1;
 *     if this makes r = 1, set R_1 <- R and terminate the algorithm.
 */
void
subroutine_5_2_3_h_heap_sort(
	int64_t *X, uint64_t n)
{
  // **H1.** [Initialize.]

  // RosettaCode start
  uint64_t l = n/2 + 1;

  // RosettaCode end
  uint64_t r = n;

  /*
   * **H2L.** [Decrease l.]
   * Knuth H2L()
   * Williams SETHEAP()
   * RosettaCode heapify()
   */
  while (l > 1) {
    // Williams INHEAP(X, j, X[j + 1])
    subroutine_5_2_3_h3u_sift_up(X, l, l, r);

    // Decrease l.
    --l;
  }

  /*
   * **H2R.** [Decrease r.]
   * Knuth H2R()
   * Williams OUTHEAP()
   * RosettaCode unheapify()
   */
  while (r > 1) {
    // Williams SWAPHEAP(X, n - 1, X[n], R)
    subroutine_5_2_3_h3u_sift_up(X, l, r, r);

    // Decrease r.
    --r;
  }
}

int main(int argc, char* argv[])
{
  if(argc > 1) {
    usage();
    exit(0);
  }

  // Read input.
  uint64_t N;
  fread(&N, sizeof N, 1, stdin);
  int64_t X[N + 1];
  fread(&X[1], sizeof(*X), N, stdin);

  // Main logic: heap sort.
  subroutine_5_2_3_h_heap_sort(X, N);

  // Write output.
  fwrite(&N, sizeof N, 1, stdout);
  fwrite(&X[1], sizeof(*X), N, stdout);
  return 0;
}
