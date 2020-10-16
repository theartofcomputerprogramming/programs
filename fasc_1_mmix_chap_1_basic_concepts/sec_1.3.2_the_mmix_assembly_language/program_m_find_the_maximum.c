// program_m_find_the_maximum.c

// Algorithm M (Find the maximum)
// Section 1.2.10 Analysis of an Algorithm, Volume 1, Fundamental Algorithms
// Section 1.3.2' The MMIX Assembly Language, Fascicle 1, MMIX

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <endian.h>

void usage()
{
  printf("program_m_find_the_maximum n\n");
  printf("prints the index and value of the maximum of 64-bit integers as binary data\n");
  printf("n: number of 64-bit integers in bigendian binary format to read from stdin\n");
  printf("Examples:\n");
  printf("program_m_find_the_maximum 100 <data.100.dat | od -An -td8 -w8 --endian=big\n");
}

// byteswap array from bigendian to littleendian if needed
void ntoh64a(uint64_t* a, const uint64_t n)
{
  if(a == NULL || n == 0)
    return;

  for(uint64_t i=0; i < n; ++i) {
    a[i] = be64toh(a[i]);
  }
}

// returns maximum of array X from index 1 through index n
// A: array of 64-bit integers
// n: last index of A starting from index 1
// m: returns max value
// j: returns index of max value

int Maximum(const int64_t* X, const uint64_t n, int64_t* m, uint64_t* j)
{
  if( X == NULL || n == 0)
    return -1;


  *j = n;                  // M1 [Initialize] j <- n, m <- X[n]
  *m = X[n];

  for(
      uint64_t k = n - 1;  // k <- n - 1
      k != 0;              // M2 [All tested?] To M3 if k != 0
      --k                  // M5 [ Decrease k] k <- k - 1
      ) {

    if(X[k] <= *m)         // M3 [Compare]
      continue;            // To M5 if X[k] <= m

    *m = X[k];             // M4 [Change m] m <- X[k]
    *j = k;                // j <- k

  }

  return 0;
}

int main(int argc, char* argv[])
{

  if(argc < 2) {
    usage();
    return 1;
  }

  char* endptr;
  const uint64_t N = strtoull(argv[1], &endptr, 10);

  if(endptr == argv[1]) {
    usage();
    return 1;
  }

// the array of 64-bit integers indexed from 1 not 0
  int64_t* X;

// allocate one more than size to use indexes 1-N
  if(!(X = malloc((N+1) * sizeof(*X)))) {
    return 2;
  }

// fill array from index 1 onward
  fread(X+1, sizeof(*X), N, stdin);

// byteswap array from bigendian to littleendian if needed
  ntoh64a((uint64_t*)X+1, N);

// max value
  int64_t max;
// index of max value
  uint64_t i_max;

  int ev = Maximum(X, N, &max, &i_max);

  free(X);

  if(ev) {
    return 3;
  }

// byteswap to bigendian if needed
  max=htobe64(max);
  i_max=htobe64(i_max);

  fwrite(&i_max, sizeof(i_max), 1, stdout);
  fwrite(&max, sizeof(max), 1, stdout);

  return 0;
}

