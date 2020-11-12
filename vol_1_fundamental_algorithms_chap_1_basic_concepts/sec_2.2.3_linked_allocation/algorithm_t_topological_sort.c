// algorithm_t_topological_sort.c

// Algorithm T (Topological sort), 2.2.3 Linked Allocation,
// The Art of Computer Programming, Donald Knuth

// Program T (Topological sort), 2.2.3 Linked Allocation,
// The MMIX Supplement, Martin Ruckert

// usage: algorithm_t_topological_sort <in.dat >out.dat
// reads sequence of pairs of binary uint32_t values from in.dat
// each pair is a dependency relation between objects
// first of each pair is predecessor, second is successor
// first pair is special: first value is 0, second is objects count
// last pair is special: both values are 0
// output is binary uint32_t values in topologically sorted order
// examples:
// algorithm_t_topological_sort <in.0.le.dat | od -An -td4 -w4 -v


#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

struct Pair {
  uint32_t left;
  uint32_t right;
};

struct Object {
  uint32_t COUNT;
  uint32_t TOP;
};

struct Successor {
  uint32_t SUC;
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
  uint32_t outcap = sizeof(Buffer)/sizeof(*OutBuf);

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

  if(TSort(stdin, stdout) != 0)
    return 1;

  return 0;
}

