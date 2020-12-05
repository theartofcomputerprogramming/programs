// algorithm_m_multiple_list_insertion.c

// Algorithm M (Multiple list insertion)
// 5.2.1 Sorting by Insertion
// The Art of Computer Programming, Donald Knuth

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void usage()
{
  puts("usage:algorithm_m_multiple_list_insertion <in.dat >out.dat");
  puts("Implements Algorithm M (Multiple list insertion), 5.2.1 Sorting by Insertion, The Art of Computer Programming Volume 3, Sorting and Searching by Donald Knuth");

  puts("reads 64-bit values as binary data to sort, outputs sorted 64-bit values as binary data");

  puts("first uint64_t is number of values to sort");
  puts("next that many int64_t is data to sort");

  puts("");
  puts("binary input data format");
  puts("uint64_t e");
  puts("uint64_t M");
  puts("uint64_t N");
  puts("uint64_t[N] data");

  puts("");
  puts("binary output data format");
  puts("uint64_t N");
  puts("uint64_t[N] sorted data");

  puts("");
  puts("examples:");
  puts("algorithm_m_multiple_list_insertion <data/algorithm_m_multiple_list_insertion/in.0.le.dat | od -An -td8 -w8 -v");
}


struct Record {
  uint64_t LINK;
  uint64_t KEY;
};

// Sort takes array R of N+1 records
// Sort implements Algorithm M (Multiple list insertion)
void Sort(const uint64_t N, struct Record R[N+1], const uint64_t M, uint64_t Heads[M], const uint64_t e)
{

// clear heads
  for(uint64_t i = 0; i < M; ++i) {
    Heads[i] = 0;
  }

// loop over unsorted records in reverse
  for(uint64_t j = N; j > 0; --j) {

// index of next sorted node to compare
// chose to keep this as index instead of making it a pointer like q since
// indexes are nicer to work with than pointers
    uint64_t p;

// trailing pointer to index of next sorted node
// this cannot easily be an offset like in mmix program because R and Heads are arrays of different types
    uint64_t* q;

// current unsorted key
    int64_t K;

    for(

// get new unsorted key, map new key to its list head, get index of first sorted key in list
      K = R[j].KEY, q = &Heads[(M * K) >> e], p = *q;

// stop when end of sorted list is reached
      p != 0;

// update q and p to next nodes in sorted list
      q = &R[p].LINK, p = *q
    ) {

// compare new unsorted key K to current sorted key K_p
      if(K <= R[p].KEY)
        break;

    }

// insert new unsorted node before current sorted node
    *q = j;
    R[j].LINK = p;

  }

}

int main(int argc, char* argv[])
{

  if(argc > 1) {
    usage();
    exit(0);
  }

  uint64_t e;
  fread(&e, sizeof e, 1, stdin);

  uint64_t M;
  fread(&M, sizeof M, 1, stdin);

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

  uint64_t Heads[M];

  Sort(N, R, M, Heads, e);

// write number of values to follow
  fwrite(&N, sizeof N, 1, stdout);

// print linked list of sorted keys as binary data
  for(uint64_t j = 0; j < M; ++j) {
    for(uint64_t i = Heads[j]; i != 0; i = R[i].LINK) {
      fwrite(&R[i].KEY, sizeof(R[i].KEY), 1, stdout);
    }
  }

  return 0;
}

