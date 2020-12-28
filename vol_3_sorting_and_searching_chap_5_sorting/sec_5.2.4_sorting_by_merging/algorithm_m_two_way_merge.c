// algorithm_m_two_way_merge.c

// Algorithm M (Two-way merge)
// 5.2.4 Sorting by Merging
// The Art of Computer Programming, Donald Knuth

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

static void usage()
{
  puts("usage:algorithm_m_two_way_merge <in.dat >out.dat");

  puts("reads 64-bit values as binary data to sort, outputs sorted 64-bit values as binary data");

  puts("first uint64_t is number of values to sort");
  puts("next that many int64_t is data to sort");

  puts("");
  puts("binary input data format");
  puts("uint64_t m, can be 0 for empty array");
  puts("int64_t[m] data");
  puts("uint64_t n, can be 0 for empty array");
  puts("int64_t[n] data");

  puts("");
  puts("binary output data format");
  puts("uint64_t N");
  puts("int64_t[N] sorted data");

  puts("");
  puts("examples:");
  puts("algorithm_m_two_way_merge <data/algorithm_m_two_way_merge/in.0.le.dat | od -An -td8 -w8 -v");
}

void merge(const uint64_t m, const uint64_t n; int64_t x[m+1], const uint64_t m, int64_t y[n+1], const uint64_t n, int64_t z[m+n+1])
{

// check empty special cases
// return empty z if both x and y are empty
  if(m == 0 && n == 0)
    return;
// copy y to z if x is empty
  if(m == 0) {
    z[1] = y[1];
    for(uint64_t i = 1; i <= n; ++i) {
      z[i] = y[i];
    }
    return;
  }
// copy x to z if y is empty
  if(n == 0) {
    for(uint64_t i = 1; i <= m; ++i) {
      z[i] = x[i];
    }
    return;
  }

// start algorithm in earnest with nonempty x and y arrays

// M1 [Initialize] i <- 1, j <- 1, k <- 1
  uint64_t i = 1, j = 1, k = 1;

  for(;;) {

// M2 [Find smaller] To M3 if x_i <= y_i, to M5 otherwise
    if(x[i] <= y[j]) {
// M3 [Output x_i] z_k <- x_i, k <- k = 1, i <- i + 1
      z[k] = x[i];
      ++k;
      ++i;
      if(i <= m) {
        continue;
      }
// M4 [Transmit y_j,...,y_n] (z_k,...,z_(m + n)) <- (y_j,...,y_n)
      for(; j <= n; ++j, ++k) {
        z[k] = y[j];
      }
// M4 [Transmit y_j,...,y_n] Terminate
      break;
    }

// M5 [Output y_j] z_k <- y_j, k <- k = 1, j <- j + 1
    z[k] = y[j];
    ++k;
    ++j;
    if( j <= n) {
      continue;
    }

// M6 [Transmit x_i,...,x_m]
    for(; i <= m; ++i, ++k) {
      z[k] = x[i];
    }
// M6 [Transmit x_i,...,x_m] Terminate
    break;

  }

}

int main(int argc, char* argv[])
{

  if(argc > 1) {
    usage();
    exit(0);
  }

  uint64_t m;
  fread(&m, sizeof m, 1, stdin);

// read array X of values as binary data
  int64_t X[m + 1];

  fread(&X[1], sizeof(*X), m, stdin);

// read 64-bit size of Y array as binary data
  uint64_t n;
  fread(&n, sizeof n, 1, stdin);

// read array Y of values as binary data
  int64_t Y[n + 1];

  fread(&Y[1], sizeof(*Y), n, stdin);

  int64_t Z[m + n + 1];

  merge(X, m, Y, n, Z);

// write number of values to follow
  const uint64_t zsize = m + n;
  fwrite(&zsize, sizeof zsize, 1, stdout);

// print sorted array as binary data
  fwrite(&Z[1], sizeof(*Z), zsize, stdout);

  return 0;
}

