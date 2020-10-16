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

  PRIME[1] = 2;       // P1 [Start table] PRIME[1] <- 2
  uint16_t n = 3;     // n <- 3
  unsigned j = 1;     // j <- 1

  for(
      PRIME[++j] = n; // P2a [n is prime] j <- j+1, PRIME[j] <- n
      j != 500;       // P3 [500 found]
      PRIME[++j] = n  // P2 [n is prime] j <- j+1, PRIME[j] <- n
      ) {

    n += 2;           // P4a [Advance n] n <- n + 2

    for(
        bool prime=false;
        ;
        n += 2        // P4 [Advance n] n <- n + 2
        ) {

      for(
          unsigned k = 2;  // P5 [k <- 2]
          ;
          ++k         // P8 [Advance k] k <- k + 1
          ) {

        uint16_t q = n / PRIME[k];  // q <- floor(n / PRIME[k])
        uint16_t r = n % PRIME[k];  // r <- n mod PRIME[k]

        if(r == 0) {  // P6 [PRIME[k]\n?]
          break;      // To P4 if r = 0
        }

        if(q <= PRIME[k]) {  // P7 [PRIMEk] large?]
          prime = true;  // see exercise 1.3.2.11
          break;      // To P2 if q <= PRIME[k]
        }
      }

      if(prime)
        break;        // To P2
    }

  }

  puts("First Five Hundred Primes");  // P9 [Print title]

  for(
      unsigned m = 1; // m <- 1
      m <= 50;        // P11 [500 printed?] To p10 if m <= 50
      ++m             // m <- m + 1
      ) {
                      // P10 [Print line]
    fputs("  ", stdout);
    for(unsigned i = 0; i < 10; ++i) {
      printf(" %.4u", PRIME[50*i + m]);
    }
    puts("");

  }

  return 0;
}

