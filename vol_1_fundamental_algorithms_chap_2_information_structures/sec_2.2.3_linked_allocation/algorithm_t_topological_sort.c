// algorithm_t_topological_sort.c

// Algorithm T (Topological sort), 2.2.3 Linked Allocation,
// The Art of Computer Programming, Donald Knuth

// Program T (Topological sort), 2.2.3 Linked Allocation,
// The MMIX Supplement, Martin Ruckert

// basic idea is to have an array Base of n objects
// each object contains info about its predecessors and successors
// an object has a linked list TOP of its successors
// an object has a count COUNT of its predecessors
// as each relation is read, the predecessors count of the successor is inncremented
// and the successor is prepended to the linked list of successors for the predecessors
// once all relations are processed, the array entries with 0 counts are the ones to start with
// so a queue QLINK with head QLINK[0] is created by reusing the COUNT fields of the objects
// this works because only objects with COUNT 0 are added to the queue ie COUNT is no longer needed
// the queue is populated with a first pass through the array adding the objects with no predecessors, here the array is traversed backwards but it can be done forwards too
// then the queue is processed
// an object is popped, its successor list is traversed, each successor has its predecessors count decremented, if the count goes to zero, the successor is added to the queue
// when object 0 is popped from the queue it means the queue was empty because the 0 is coming from the underlying COUNT value that turned 0 and was not overwritten with an object index when an object is added to the queue
// a counter N is also kept while processing the queue to insure all objects have been output, if the counter does not reach 0 it means there was a loop in the dependency relations otherwise it was a successful topological sort

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static void usage(void)
{
  puts("usage: algorithm_t_topological_sort <in.dat >out.dat");
  puts("Implements Algorithm T (Topological sort) from 2.2.3 Linked Allocation, The Art of Computer Programming Volume 1, Fundamental Algorithms by Donald Knuth");
  puts("Input and output is binary compatible with Program T (Topological sort) from 2.2.3 Linked Allocation, The MMIX Supplement by Martin Ruckert");
  puts("");
  puts("Reads sequence of pairs of binary uint32_t values on stdin");
  puts("Each pair is a dependency relation between objects");
  puts("First of each pair is predecessor, second is successor");
  puts("First pair is special: first value is 0, second is objects count");
  puts("Last pair is special: both values are 0");
  puts("Output on stdout is binary uint32_t values in topologically sorted order");
  puts("");
  puts("Examples:");
  puts("algorithm_t_topological_sort <in.0.le.dat | od -An -td4 -w4 -v");
}

// Pair represents input predecessor-successor relation between objects
struct Pair {
// left is left of pair, index of predecessor object
  uint32_t left;
// right is right of pair, index of successor object
  uint32_t right;
};

// Object represents a single object with info about its predecessors and successors
// Object is an entry in an array
struct Object {
// COUNT is number of direct predecessors of object k
  uint32_t COUNT;
// TOP is link to list of direct successors of object k
  uint32_t TOP;
};

// Successor has info about a single successor of an object
// Successor is a node in a linked list
struct Successor {
// SUC is index of a direct successor of object k
  uint32_t SUC;
// NEXT is link to next item in successors list
  uint32_t NEXT;
};

int TSort(FILE* Fin, FILE* Fout)
{

// input pairs buffer capacity, 256 pairs
#define SIZE 256u

// input pairs buffer, zero sentinel value at end
  struct Pair Buffer[SIZE + 1] = {0};

  fread(Buffer, sizeof(*Buffer), SIZE, Fin);

  uint32_t n = Buffer[0].right;    // T1 [Initialize] Input the value of n

// array of n objects plus special object 0
  struct Object* Base = malloc((n + 1) * sizeof(*Base));

// queue of topologically sorted objects overlaid on COUNT fields of objects list
#define QLINK(qlink_i) (Base[(qlink_i)].COUNT)

// storage pool for linked list nodes
  uint32_t POOLSIZE = 2*n;
  struct Successor* Pool = malloc(POOLSIZE * sizeof(*Pool));

// AVAIL is offset of first available node in storage pool
// AVAIL uses simple scheme of incrementing offset after available node is taken
// AVAIL must be greater than zero since zero is lambda/null pointer
  uint32_t AVAIL = 1;

// T1 [Initialize] COUNT[k] <- 0, TOP[k] = Lambda, for 1<= k <= n
  for(uint32_t k = 1; k <= n; ++k) {
    Base[k].COUNT = 0;
    Base[k].TOP = 0;
  }

// T1 [Initialize] N <- n
  uint32_t N = n;

// fill objects array and successor lists
  for(

// T2 [Next relation] Next relation (j,k) from input
    uint32_t i = 1, j = Buffer[i].left, k = Buffer[i].right;

// T2 [Next relation] To T4 if input exhausted
    j != 0 || i >= SIZE;

// T2 [Next relation] Next relation (j,k) from input
    ++i, j = Buffer[i].left, k = Buffer[i].right
  ) {

// refill buffer with more input pairs
    if(i >= SIZE) {
      fread(Buffer, sizeof(*Buffer), SIZE, Fin);
      i = 0;
    }

// T3 [Record the relation] COUNT[k] <- COUNT[k] + 1
    ++Base[k].COUNT;

// T3 [Record the relation] P <= AVAIL
    uint32_t P = AVAIL++;
    if(AVAIL > POOLSIZE) {
      POOLSIZE *= 2;
      Pool = realloc(Pool, POOLSIZE);
    }

// T3 [Record the relation] SUC(P) <- k, NEXT(P) <- TOP[j], TOP[j] <- P
    Pool[P].SUC = k;
    Pool[P].NEXT = Base[j].TOP;
    Base[j].TOP = P;

// T3 [Record the relation] To T2
  }

// T4 [Scan for zeros] R <- 0, QLINK[0] <- 0
  uint32_t R = 0;
  QLINK(0) = 0;

// T4 [Scan for zeros] QLINK[R] <- k, R<- k, if COUNT[k] = 0, for 1 <= k <= n
// reverse iteration for output compatibility with MMIX program
  for(uint32_t k = n; k > 0; --k) {
    if(Base[k].COUNT != 0)
      continue;
    QLINK(R) = k;
    R = k;
  }

// reuse input buffer for output
  uint32_t* OutBuf = (void*)Buffer;
// parentheses around sizeof in denominator suppress warning about
// mismatched types of Buffer and OutBuf
  uint32_t outcap = sizeof(Buffer)/(sizeof(*OutBuf));

// track output buffer capacity
  uint32_t i = 0;

// output
  for(

// T4 [Scan for zeros] F <- QLINK[0]
    uint32_t F = QLINK(0);

// T5 [Output front of queue] To T8 if F = 0
    F != 0;

    F = QLINK(F)
   ) {

    OutBuf[i++] = F;
    if(i >= outcap) {
      fwrite(OutBuf, sizeof(*OutBuf), outcap, Fout);
      i = 0;
    }

    --N;

 // update successors
    for(
      uint32_t P = Base[F].TOP;
      P != 0;
      P = Pool[P].NEXT
    ) {

      if(--Base[Pool[P].SUC].COUNT != 0) {
        continue;
      }

      QLINK(R) = Pool[P].SUC;
      R = Pool[P].SUC;

    }

  }

  OutBuf[i++] = 0;

  fwrite(OutBuf, sizeof(*OutBuf), i, Fout);

  free(Base);
  free(Pool);

  return N;
}

int main(int argc, char* argv[])
{
  (void)argv;

  if(argc > 1) {
    usage();
    return 1;
  }

  if(TSort(stdin, stdout) != 0)
    return 1;

  return 0;
}

