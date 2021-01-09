// algorithm_l_list_merge_sort.signbit.c 

// Algorithm L (List merge sort)
// 5.2.4 Sorting by Merging
// The Art of Computer Programming, Donald Knuth
// The MMIX Supplement, Martin Ruckert

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


static void usage()
{
  puts("usage:algorithm_l_list_merge_sort.signbit <in.dat >out.dat");
  puts("Implements Algorithm L (List merge sort), 5.2.4 Sorting by Merging, The Art of Computer Programming Volume 3, Sorting and Searching by Donald Knuth");

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
  puts("algorithm_l_list_merge_sort.signbit <data/algorithm_l_list_merge_sort.signbit/in.0.le.dat | od -An -td8 -w8 -v");
}

struct Record {
// link field, double to use ieee-754 sign bit
  double L;
// key field
  int64_t K;
};

// Sort takes array R of N + 2 records
// Sort implements Algorithm L (List merge sort)
// K is sorted in place
void Sort(const uint64_t N; struct Record R[N + 2], const uint64_t N)
{

// boundary cases
  if(N == 0) {
    R[0].L = 0;
    return;
  }

  if(N == 1) {
    R[0].L = 1;
    R[1].L = 0;
    return;
  }

// algorithm assumes N >= 2

// L1 [Prepare two lists] L_0 <- 1, L_(N + 1) <- 2
// L_0 is head of list with odd indexes, R_1, R_3, R_5, ...
  R[0].L = 1;

// L_(N + 1) is head of list with even indexes, R_2, R_4, R_6, ...
  R[N + 1].L = 2;

// L1 [Prepare two lists] L_i <- -(i + 2) for 1 <= i <= N - 2
  for(int64_t i = 1; i + 2 <= (int64_t)N; ++i) {
    R[i].L = -(i + 2);
  }

// L1 [Prepare two lists] L_(N -  1) <- L_N <- 0
// R_(N - 1) and R_N are the end of the two lists
  R[N - 1].L = R[N].L = 0;

  for(;;) {

// L2 [Begin new pass] s <- 0, t <- N + 1, p <- L_s, q <- L_t
// the heads of the two lists
    uint64_t s = 0;
    uint64_t t = N + 1;

    int64_t p = R[s].L;

    int64_t q = R[t].L;

// L2 [Begin new pass] Terminate if q = 0
    if(q == 0)
      return;

    for(;;) {

// L3 [Compare K_p:K_q] To L6 if K_p > K_q
      if(R[p].K > R[q].K) {

// L6 [Advance q] |L_s| <- q, s <- q, q <- L_q
        R[s].L = copysign(q, R[s].L);
        s = q;
        q = R[q].L;

// L6 [Advance q] To L3 if q > 0
        if(q > 0)
          continue;

// L7 [Complete the sublist] L_s <- p, s <- t
        R[s].L = p;
        s = t;

// L7 [Complete the sublist] t <- p, p <- L_p till p <= 0
        do {
          t = p;
          p = R[p].L;
        } while(p > 0);

      } else {

// L4 [Advance p] |L_s| <- p, s <- p, p <- L_p
        R[s].L = copysign(p, R[s].L);
        s = p;
        p = R[p].L;

// L4 [Advance p] To L3 if p > 0
        if(p > 0)
          continue;

// L5 [Complete the sublist] L_s <- q, s <- t
        R[s].L = q;
        s = t;

// L5 [Complete the sublist] t <- q, q <- L_q till q <= 0
        do {
          t = q;
          q = R[q].L;
        } while(q > 0);

// L5 [Complete the sublist] To L8
      }

// L8 [End of pass?] p <- -p, q <- -q
      p = -p;
      q = -q;

// L8 [End of pass?] |L_s| <- p, |L_t| <- 0
      if(q == 0) {
        R[s].L = copysign(p, R[s].L);
        //R[t].L = 0;
        R[t].L = copysign(0, R[t].L);
// L8 [End of pass?] To L2
        break;
      }

// L8 [End of pass?] To L3

    }

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
// allocate N+2 entries for extra artificial records at index 0 and N+1
  struct Record R[N + 2];

// fill key field of each record
  for(uint64_t i = 1; i <= N; ++i) {
    fread(&R[i].K, sizeof(R[i].K), 1, stdin);
  }

  Sort(R, N);

// write number of values to follow
  fwrite(&N, sizeof N, 1, stdout);


// traverse linked list to print sorted keys as binary data
  for(uint64_t i = R[0].L; i != 0; i = R[i].L) {
    fwrite(&R[i].K, sizeof(R[i].K), 1, stdout);
  }

  return 0;
}

