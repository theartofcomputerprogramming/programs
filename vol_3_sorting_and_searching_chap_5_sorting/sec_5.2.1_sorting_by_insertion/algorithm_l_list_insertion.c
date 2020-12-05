// algorithm_l_list_insertion.c

// Algorithm L (List insertion)
// 5.2.1 Sorting by Insertion
// The Art of Computer Programming, Donald Knuth

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void usage()
{
  puts("usage:algorithm_l_list_insertion <in.dat >out.dat");
  puts("Implements Algorithm L (List insertion), 5.2.1 Sorting by Insertion, The Art of Computer Programming Volume 3, Sorting and Searching by Donald Knuth");

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
  puts("algorithm_l_list_insertion <data/algorithm_l_list_insertion/in.0.le.dat | od -An -td8 -w8 -v");
}

struct Record {
  uint64_t LINK;
  int64_t KEY;
};

// Sort takes array R of N+1 records
// Sort implements Algorithm L (List insertion)
void Sort(const uint64_t N, struct Record R[N+1])
{

// L1 [Loop on j] L_0 <- N, L_N <- 0
  R[0].LINK = N;
  R[N].LINK = 0;

// L1 [Loop on j] Perform L2 through L5 for j = N-1, N-2,..., 1
  for(uint64_t j = N - 1; j >= 1; --j) {

    uint64_t p, q;
    int64_t K;

    for(

// L2 [Set up p, q, K] p <- L_0, q <- 0, K <- K_j
      p = R[0].LINK, q = 0, K = R[j].KEY;

// L4 [Bump p, q] To L3 if p > 0
      p != 0;

// L4 [Bump p, q] q <- p, p <- L_q
      q = p, p = R[q].LINK
    ) {

// L3 [Compare K:K_p] To L5 if K <= K_p
      if(K <= R[p].KEY)
        break;

    }

// L5 [Insert into list] L_q <- j, L_j <- p
    R[q].LINK = j;
    R[j].LINK = p;

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
// allocate N+1 entries with special node at index 0
  struct Record R[N + 1];

// special record at index 0 for head of linked list of sorted nodes
  R[0].LINK = 0;
  R[0].KEY = 0;

  for(uint64_t i = 1; i <= N; ++i) {
    fread(&R[i].KEY, sizeof(R[i].KEY), 1, stdin);
  }

  Sort(N, R);

// write number of values to follow
  fwrite(&N, sizeof N, 1, stdout);

// print linked list of sorted keys as binary data
  for(uint64_t i = R[0].LINK; i != 0; i = R[i].LINK) {
    fwrite(&R[i].KEY, sizeof(R[i].KEY), 1, stdout);
  }


  return 0;
}

