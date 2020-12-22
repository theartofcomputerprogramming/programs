// algorithm_r_radix_exchange_sort.c

// Algorithm R (Radix exchange sort)
// 5.2.2 Sorting by Exchanging
// The Art of Computer Programming, Donald Knuth
// uses stack as described in algorithm

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// algorithm uses a stack to accumulate right partitions and defer their
// processing till a left partition is completely processed
// this is why there is no need to keep the left boundary of a partition
// in the entry stored on the stack

// one stage of the algorithm scans a partition first rightward from the left then leftward from the right looking for pairs of keys to swap
// each key is tested for a specific bit number in one stage
// a key with a 1 bit found in the scan from the left is swapped with a key
// with a 0 bit found in the scan from the right
// i.e. goal is to keep smaller keys (0 bit) on the left and bigger keys (1 bit) on the right splitting the partition into two sections

static void usage()
{
  puts("usage:algorithm_r_radix_exchange_sort <in.dat >out.dat");

  puts("reads 64-bit values as binary data to sort, outputs sorted 64-bit values as binary data");

  puts("first uint64_t is max number of bits needed for values");
  puts("second uint64_t is number of values to sort");
  puts("next that many int64_t is data to sort");

  puts("");
  puts("binary input data format");
  puts("uint64_t m");
  puts("uint64_t N");
  puts("int64_t[N] data");

  puts("");
  puts("binary output data format");
  puts("uint64_t N");
  puts("int64_t[N] sorted data");

  puts("");
  puts("examples:");
  puts("algorithm_r_radix_exchange_sort <data/algorithm_r_radix_exchange_sort/in.0.le.dat | od -An -td8 -w8 -v");
}

// entry object of partition parameters to keep on stack
struct entry_t {

// right boundary of partition
  uint64_t r;

// bit number to check for each key in partition
  uint64_t b;
};

// Sort takes array K of N unsigned keys beginning at K[1]
// Sort implements Algorithm R (Radix exchange sort)
// K is sorted in place
// m is max number of bits needed for range of keys
void Sort(const uint64_t N, uint64_t K[N + 1], const uint64_t m)
{

// R1 [initialize]

// stack of partition entries of size m - 1 according to algorithm
  const uint64_t STACK_MAX = m - 1;
  struct entry_t stack[STACK_MAX];
  uint64_t STACK_SIZE = 0;

// left boundary of partition
  uint64_t l = 1;
// right boundary of partition
  uint64_t r = N;
// bit number to test, bits are numbered from right starting with 0 unlike taocp that numbers bits from left starting with 1
  uint64_t b = 1ul << (m - 1);

// runs while stack of right partitions is not empty
// terminates by break below
  for(;;) {

// R2 [Begin new stage] To R10 if l = r
    for(; l != r;) {

// R2 [Begin new stage] i <- l, j <- r
      uint64_t i = l;
      uint64_t j = r;

      bool found_swap_pair = false;

// R4 [Increase i]
      for(
// insure loop is entered at least once
        bool R3_first_time = true;

// R4 [Increase i] To R3 if i <= j, to R8 otherwise
        R3_first_time || i <= j;

// R4 [Increase i] i <- i + 1
        ++i) {

        if(R3_first_time)
          R3_first_time = false;

// R3 [Inspect K_i for 1] To R6 if bit b of K_i is 1
        if((K[i] & b) == 0) {
          continue;
        }

        for(--j; i <= j; --j) {

// R5 [Inspect K_(j + 1) for 0] To R7 if bit b of K_(j + 1) is 0
          if((K[j + 1] & b) == 0) {
            found_swap_pair = true;
            break;
          }
        }

// R6 [Decrease j] To R8 if i > j
        if(!found_swap_pair) {
          break;
        }

// R7 [Exchange R_i, R_(j + 1)]
        uint64_t tmp = K[i];
        K[i] = K[j + 1];
        K[j + 1] = tmp;

        found_swap_pair = false;

      }

// one partitioning stage has completed

// R8 [Test special cases] b <- b + 1
      b >>= 1;

// R8 [Test special cases] To R10 if b > m where m is number of bits in keys
      if(b == 0) {
        break;
      }

// R8 [Test special cases] To R2 if j < l or j = r
      if(j < l || j == r) {
        continue;
      }

// R8 [Test special cases] To R2 if j = l, with l <- l + 1
      if(j == l) {
        ++l;
        continue;
      }

// R9 [Put on stack] (r, b) => stack, to R2 with r <- j
// abort on stack overflow
      if(++STACK_SIZE >= STACK_MAX) {
        abort();
      }
      stack[STACK_SIZE - 1] = (struct entry_t){r, b};

      r = j;

    }

// R10 [Take off stack] Done if stack is empty
    if(STACK_SIZE == 0) {
      break;
    }

// R10 [Take off stack] To R2, l <- r + 1, (r', b') <= stack, r <- r', b <- b'
    l = r + 1;

    struct entry_t top = stack[STACK_SIZE - 1];
    --STACK_SIZE;

    r = top.r;
    b = top.b;

  }

}

int main(int argc, char* argv[])
{

  if(argc > 1) {
    usage();
    exit(0);
  }

// read 64-bit max number of bits as binary data
  uint64_t m;
  fread(&m, sizeof m, 1, stdin);

// read 64-bit size of data array as binary data
  uint64_t N;
  fread(&N, sizeof N, 1, stdin);

// read array R of records as binary data
  uint64_t R[N + 1];

  fread(&R[1], sizeof(*R), N, stdin);

  Sort(N, R, m);

// write number of values to follow
  fwrite(&N, sizeof N, 1, stdout);

// print sorted array as binary data
  fwrite(&R[1], sizeof(*R), N, stdout);

  return 0;
}



