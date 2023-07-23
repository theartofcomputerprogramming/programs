// SPDX-FileCopyrightText: © 2023 Andrew Robbins
// SPDX-License-Identifier: MIT

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define LIST_SWAP(X, i, j, t) \
  if (i != j) {		      \
    t = X[i];		      \
    X[i] = X[j];	      \
    X[j] = t;		      \
  }

static void usage()
{
  puts("usage: algorithm_s_straight_selection_sort < in.dat > out.dat");
}

/*
 * (Note that unlike almost every other algorithm-program
 * pair in TAOCP, the section numbers of these two do not
 * match. We could have picked either of these section
 * numbers for the name of the subroutine below,
 * and so we chose the one with the number 3 in it.)
 *
 * **Program 1.3.2M** (*Find the maximum*)
 *
 * (Omitted because we are focusing on C.)
 *
 * **Algorithm 1.2.10M** (*Find the maximum*)
 *
 * Given `n` elements `X[1], X[2], ..., X[n]`,
 * we will find `m` and `j` such that `m = X[j]`,
 * where `j` is the largest such index that
 * satisfies this relation.
 *
 * **M1.** [Initialize.] Set `j <- n`, `k <- n - 1`, `m <- X[n]`. 
 *      (During this algorithm we will have `m = X[j]`.)
 * **M2.** [All tested?] If `k = 0`, the algorithm terminates.
 * **M3.** [Compare.] If `X[k] <= m`, go to M5.
 * **M4.** [Change `m`.] Set `j <- k`, `m <- X[k]`. 
 *      (This value of m is a new current maximum.)
 * **M5.** [Decrease `k`.] Decrease `k` by one and return to M2.
 */
void
subroutine_1_3_2_find_the_maximum(
	int64_t *X,
	uint64_t n,
	int64_t *max_value,
	uint64_t *max_index)
{
  // **M1.** [Initialize.]
  int64_t j, k;
  j = k = n;
  int64_t m = X[j];

  // In TAOCP, this is represented by the step M1,
  // which says `j <- n`, `k <- n - 1`, and so
  // `k` must be one less than `j`.
  --k;

  // **M2.** [All tested?]
  while(k >= 1) {
    // **M3.** [Compare.]
    if (m < X[k]) {
      // **M4.** [Change m.]
      m = X[j = k];
    }
    // **M5.** [Decrease k.]
    --k;
  }

  *max_value = m;
  *max_index = j;
}

/*
 * **Program 5.2.3S** (*Straight selection sort*)
 *
 * (Omitted because we are focusing on C)
 *
 * **Algorithm 5.2.3S** (*Straight selection sort*)
 *
 * Records `R_1, ..., R_N` are rearranged in place;
 * after sorting is complete, their keys will be
 * in order `K_1 <= ... <= K_N`.
 *
 * **S1.** [Loop on j.] Perform steps S2 and S3 for
 *      `j = N, N-1, ..., 2`.
 * **S2.** [Find `max(K_1, ..., K_j)`.] Search through keys
 *      `K_j, ..., K_1` to find the maximal one; let it be `K_i`,
 *      where `i` is as large as possible.
 * **S3.** [Exchange with `R_j`.] Interchange records
 *      `R_i <-> R_j`. (Now records `R_j, ..., R_N` are in their
 *      final position.)
 */
void
subroutine_5_2_3_straight_selection_sort(
	int64_t *X,
	uint64_t n)
{
  uint64_t i, j;
  int64_t m, t;
  j = n;

  // **S1.** [Loop on `j`.]
  while (j >= 2) {
    /*
     * **S2.** [Find `max(K_1, ..., K_j)`.]
     *
     * This renames the variables
     * from Algorithm 5.2.3S to Algorithm 1.2.10M.
     *
     * M.X == S.X
     * M.m == S.m
     * M.n == S.j -- confusing!
     * M.j == S.i -- confusing!
     */
    subroutine_1_3_2_find_the_maximum(
	X, j, &m, &i);

    // **S3.** [Exchange with `R_j`]
    LIST_SWAP(X, i, j, t);

    /*
     * Decrement j.
     *
     * This is implied by Knuth Algorithm 5.2.3S,
     * step (S1) [Loop on `j`.] but it is not very
     * explicit. It is also referenced by Knuth
     * Program 5.2.3S, on line 14: `DEC1 1`, but
     * this line is uncommented.
     */
    --j;
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

  // Main logic: selection sort.
  subroutine_5_2_3_straight_selection_sort(X, N);
  
  // Write output.
  fwrite(&N, sizeof N, 1, stdout);
  fwrite(&X[1], sizeof(*X), N, stdout);
  return 0;
}
