// algorithm_a_addition_of_polynomials.c

// Algorithm A (Addition of polynomials), 2.2.4 Circular Lists,
// The Art of Computer Programming, Donald Knuth

// Program A (Addition of polynomials), 2.2.4 Circular Lists,
// The MMIX Supplement, Martin Ruckert

// usage: algorithm_a_addition_of_polynomials <in.dat >out.dat
// reads two polynomials as binary data on stdin
// format for each polynomial is sequence of terms terminated by single int64_t zero
// each term is int64_t coefficient followed by single int64_t for the exponents of the four variables in the term
// each exponent uses 2 bytes except the first exponent which is allowed only 15 bits of the 2 high bytes excluding the high sign bit
// examples:
// algorithm_a_addition_of_polynomials <in.0.le.dat | od -An -td8 -w8 -v
// use https://github.com/theartofcomputerprogramming/programs/blob/main/tools/texttobinary.sh to create binary data from text

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "polynomial.h"

int Add(const struct Term* P, struct Term* Q)
{
  struct Term* Q1;
  struct Term* Q2;

  for(
// A1 [Initialize] P <- LINK(P), Q1 <- Q, Q <- LINK(Q)
    Q1 = Q, P = P->LINK, Q = Q->LINK;
    ;
  ) {

// A2 [ABC(P):ABC(Q)] Repeat Q1 <- Q, Q <- LINK(Q) if ABC(P) < ABC(Q)
    while(P->ABC < Q->ABC) {
      Q1 = Q;
      Q = Q->LINK;
    }

// A3 [Add coefficients] Found terms with equal exponents
    if(P->ABC == Q->ABC) {

// A3 [Add coefficients] Terminate if ABC(P) < 0
// I'd prefer this to be the loop guard for nicer structure but it deviates from the algorithm and adds an unnecessary check for all cases
      if(P->ABC < 0)
        break;

// A3 [Add coefficients] COEF(Q) <- COEF(Q) + COEF(P)
      Q->COEF += P->COEF;

// A4 [Delete zero term]
      if(Q->COEF == 0) {
        Q2 = Q;
        Q1->LINK = Q = Q->LINK;
        free(Q2);
        P = P->LINK;
// A4 [Delete zero term] To A2
        continue;
      }

// A3 [Add coefficients] P <- LINK(P), Q1 <- Q, Q <- LINK(Q)
      P = P->LINK;
      Q1 = Q;
      Q = Q->LINK;
// A3 [Add coefficients] To A2
      continue;
    }

// A5 [Insert new term] Q2 <= AVAIL
    Q2 = malloc(sizeof *Q2);
    if(!Q2) {
      return -1;
    }

// A5 [Insert new term] COEF(Q2) <- COEF(P), ABC(Q2) <- ABC(P), ...
    Q2->COEF = P->COEF;
    Q2->ABC = P->ABC;
    Q2->LINK = Q;
    Q1->LINK = Q2;
    Q1 = Q2;
    P = P->LINK;
// A5 [Insert new term] To A2
  }

  return 0;

}

int main(int argc, char* argv[])
{

  struct Term P = {NULL, -1, 0};
  struct Term Q = {NULL, -1, 0};

  struct Term* polys[] = {&P, &Q};
  if(get_polynomials(stdin, polys, 2) != 0)
    return 1;

  if(Add(&P, &Q) != 0)
    return 2;

  polynomial_write(&Q, stdout);

  return 0;
}

