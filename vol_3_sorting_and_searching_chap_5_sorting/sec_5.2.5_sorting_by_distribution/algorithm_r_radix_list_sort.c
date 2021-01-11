// algorithm_r_radix_list_sort.c

// Algorithm R (Radix list sort)
// 5.2.5 Sorting by Distribution
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
  puts("usage:algorithm_r_radix_list_sort <in.dat >out.dat");
  puts("Implements Algorithm R (Radix list sort), 5.2.5 Sorting by Distribution, The Art of Computer Programming Volume 3, Sorting and Searching by Donald Knuth");

  puts("reads 64-bit values as binary data to sort, outputs sorted 64-bit values as binary data");

  puts("first uint64_t is base or radix M to use, currently only 256 is supported");
  puts("second uint64_t is length p of values in base M, e.g. p = 2 means values fit into 2 bytes");
  puts("third uint64_t is number of values to sort");
  puts("next that many int64_t is data to sort");

  puts("");
  puts("binary input data format");
  puts("uint64_t M");
  puts("uint64_t p");
  puts("uint64_t N");
  puts("int64_t[N] data");

  puts("");
  puts("binary output data format");
  puts("uint64_t N");
  puts("int64_t[N] sorted data");

  puts("");
  puts("examples:");
  puts("algorithm_r_radix_list_sort <data/algorithm_r_radix_list_sort/in.0.le.dat | od -An -td8 -w8 -v");
}

struct Record {
// link field, must be first for queues to work
  struct Record* LINK;
// key field
  uint64_t KEY;
};

// implements Algorithm H (Hooking-up of queues)
// takes M queues with nodes pointing from head to tail
// head of each queue is at BOTM[i]
// tail of each queue is at TOP[i]
// links all M queues together into single queue with tail at BOTM[0] and TOP[0] pointing to a node at BOTM[j] where j > i is the next nonempty queue
// the case of empty queue 0 is tricky and relies on the special way front and rear pointers are defined for these queues
static void Hook( const uint64_t M; struct Record* TOP[M], struct Record* BOTM[M], const uint64_t M)
{

// H1 [Initialize] i <- 0
  uint64_t i = 0;

// loop over M queues starting with queue 0
// 
  for(;;) {

// H2 [Point to top of pile] P <- TOP[i]
// tail of current queue
    struct Record* P = TOP[i];

// skip empty queues
    do {

// H3 [Next pile] i <- i + 1
      ++i;

// H3 [Next pile] LINK(P) <- lambda and terminate if i = M
      if(i == M) {
        P->LINK = NULL;
        return;
      }

// H4 [Is pile empty?] To H3 if BOTM[i] = lambda
    } while(BOTM[i] == NULL);

// H5 [Tie pile together] LINK(P) <- BOTM[i]
// makes tail node of current queue point to head node of higher indexed nonempty queue
    P->LINK = BOTM[i];

// H5 [Tie pile together] To H2
  }

}

// Sort takes array R of N records
// M is radix or base
// p is length of key in base M
// Sort implements Algorithm R (Radix list sort)
// R is sorted in place
struct Record* Sort(const uint64_t N; struct Record R[N + 1], const uint64_t N, const uint64_t M, const uint64_t p)
{

  if(N == 0)
    return NULL;

// R1 [Loop on k] P <- LOC(R_N)
  struct Record* P = &R[N];

// R1 [Loop on k] Perform R2-R6 for k = 1,2,...,p
  for(uint64_t k = 1; k <= p; ++k) {

// R2 [Set piles empty]

// queue design follows section 2.2.3 linked allocation pp.259-261
// node links point from head of queue to rear of queue
// crucial aspect of empty queue: "the value of variable F must be stored in the LINK field of its location if this idea is to work"
// this means F or front pointer to an empty queue is used as if it were a node with a LINK field, i.e. cast as struct Record* in our case

// head of queue
    struct Record* BOTM[M];

// rear of queue where new node enters
    struct Record* TOP[M];

// R2 [Set piles empty] TOP[i] <- LOC(BOTM[i]), BOTM[i] <- lambda for 0 <= i < M
    for(uint64_t i = 0; i < M; ++i) {

// empty queue needs BOTM[i] treated as if it were a record node
// see note above about empty queue design
      TOP[i] = (void*)&BOTM[i];
      BOTM[i] = NULL;
    }

    for(;;) {

// R3 [Extract kth digit of key] i <- kth least significant digit of key
// this assumes base 256 following the mmix implementation, may generalize later
      const uint8_t i = ((const uint8_t*)&P->KEY)[k - 1];

// R4 [Adjust links] LINK(TOP[i] <- P, TOP[i] <- P
// note TOP[i] is BOTM[i] when queue is empty
// so both TOP[i] and BOTM[i] point to the same first record node when it's inserted
      TOP[i]->LINK = P;
      TOP[i] = P;

// R5 [Step to next record] P <- LOC(R_(j - 1) if k = 1 and P = LOC(R_j) for j != 1
      if(k == 1 && P != NULL) {
        ptrdiff_t j = P - R;
        if(j != 1) {
          P = &R[j - 1];
// R5 [Step to next record] To R3
          continue;
        }
      }

// R5 [Step to next record] P <- LINK(P) if k > 1
      if(k > 1) {
        P = P->LINK;

// R5 [Step to next record] To R3 if P != lambda
        if(P != NULL)
          continue;
      }

      break;
    }

// R6 [Do Algorithm H] Perform Algorithm H
    Hook(TOP, BOTM, M);

// R6 [Do Algorithm H] P <- BOTM[0]
    P = BOTM[0];

  }

  return P;

}

int main(int argc, char* argv[])
{

  (void)argv;

  if(argc > 1) {
    usage();
    exit(0);
  }

// M is radix
  uint64_t M;
  fread(&M, sizeof M, 1, stdin);

  if(M != 256) {
    fprintf(stderr, "Invalid input data: radix M must be 256\n");
    usage();
    exit(1);
  }

// p is key length in base M
  uint64_t p;
  fread(&p, sizeof p, 1, stdin);

// read 64-bit size of data array as binary data
  uint64_t N;
  fread(&N, sizeof N, 1, stdin);

// read array R of records as binary data
  struct Record R[N + 1];

// fill key field of each record
  for(uint64_t i = 1; i <= N; ++i) {
    fread(&R[i].KEY, sizeof(R[i].KEY), 1, stdin);
  }

  const struct Record* const sorted = Sort(R, N, M, p);

// write number of values to follow
  fwrite(&N, sizeof N, 1, stdout);


// traverse linked list to print sorted keys as binary data
  for(const struct Record* p = sorted; p != NULL; p = p->LINK) {
    fwrite(&p->KEY, sizeof(p->KEY), 1, stdout);
  }

  return 0;
}


