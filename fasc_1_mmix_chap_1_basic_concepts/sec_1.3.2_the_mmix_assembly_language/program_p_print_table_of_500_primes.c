// program_p_print_table_of_500_primes.c

// Algorithm P (Print table of 500 primes)
// Section 1.3.2' The MMIX Assembly Language, Fascicle 1, MMIX

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{

  const unsigned L = 500;

// one more than L to start index at 1
  uint16_t PRIME[L+1];

  PRIME[1] = 2;
  uint16_t n = 3;
  unsigned j = 1;

  for(PRIME[++j] = n; j != 500; PRIME[++j] = n) {

// try next odd number
    n += 2;

// keep trying odd numbers till prime is found
    for(bool prime=false;; n += 2) {

// find a prime divisor starting with 3
      for(unsigned k = 2;; ++k) {

        uint16_t q = n / PRIME[k];
        uint16_t r = n % PRIME[k];

// not prime - found prime divisor
        if(r == 0) {
          break;
        }
// prime
        if(q <= PRIME[k]) {
          prime = true;
          break;
        }
// unknown if prime or not - try next prime divisor
      }

      if(prime)
        break;
    }

  }

  puts("First Five Hundred Primes");

// 10 primes per line, zero-pad to 4 digits
  for(unsigned m = 1; m <= 50; ++m) {
    fputs("  ", stdout);
    for(unsigned i = 0; i < 10; ++i) {
      printf(" %.4u", PRIME[50*i + m]);
    }
    puts("");
  }

  return 0;
}

