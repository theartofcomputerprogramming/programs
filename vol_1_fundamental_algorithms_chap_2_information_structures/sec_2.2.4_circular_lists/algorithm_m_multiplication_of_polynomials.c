// algorithm_m_multiplication_of_polynomials.c

// Algorithm M (Multiplication of polynomials), 2.2.4 Circular Lists,
// The Art of Computer Programming, Donald Knuth

// usage: algorithm_m_multiplication_of_polynomials <in.dat >out.dat
// reads three polynomials as binary data on stdin
// format for each polynomial is sequence of terms terminated by single int64_t zero
// each term is int64_t coefficient followed by single int64_t for the exponents of the four variables in the term
// each exponent uses 2 bytes except the first exponent which is allowed only 15 bits of the 2 high bytes excluding the high sign bit
// examples:
// algorithm_m_multiplication_of_polynomials <mult.in.2.le.dat | od -An -td8 -w8 -v
// use https://github.com/theartofcomputerprogramming/programs/blob/main/tools/texttobinary.sh to create binary data from text

// examples:
// https://en.wikipedia.org/wiki/Multiplication_algorithm
// (14ac - 3ab + 2) * (ac - ab + 1) = 14a^2c^2 - 17a^2bc + 16ac + 3a^2b^2 - 5ab  + 2
// computation written in sorted order by exponents:
// (-3xy + 14xz + 2) * (-xy + xz + 1)
// Q = 3x^2y^2 -3x^2yz - 3xy
//    -14x^2yz + 14x^2z^2 + 14xz
// Q = 3x^2y^2 - 17x^2yz + 14x^2z^2 - 3xy + 14xz
//     -2xy + 2xz + 2
// Q = 3x^2y^2 - 17x^2yx + 14x^2z^2 - 5xy + 16xz + 2

// https://www.math.auckland.ac.nz/class255/08s1/01s2/polynomials.pdf
// x^4 + 5x^3 - 3x^2 + x + 2 = (x^2 + 2x - 14) * (x^2 + 3x + 5) + (33x + 72)

// https://math.stackexchange.com/questions/2167213/polynomial-division-in-3-variables
// 4y - yz + (x^2 - yz + 2y) * (x - 2) = x^3 - xyz - 2x^2 + 2xy + yz
// sorted order: -yz + 4y + (x^2 - yz + 2y) * (x - 2) = x^3 - 2x^2 - xyz + 2xy + yz

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "polynomial.h"

int Mult(struct Term* Q, const struct Term* M,  const struct Term* P)
{
  for(
// M1 [Next multiplier] M <- LINK(M)
    M = M->LINK;

// M1 [Next multiplier] Terminate if ABC(M) < 0
    M->ABC >= 0;

// M2 [Multiply cycle] To M1
    M = M->LINK
  ) {

// M2 [Multiply cycle] Perform Algorithm A

    struct Term* Q1;
    struct Term* Q2;

    for(
// A1 [Initialize] P <- LINK(P), Q1 <- Q, Q <- LINK(Q)
      Q1 = Q, P = P->LINK, Q = Q->LINK;
      ;
      ) {

// M2 [Multiply cycle] replace "ABC(P)" with "(if ABC(P) < 0 then -1, otherwise ABC(P) + ABC(M))"
// A2 [ABC(P):ABC(Q)] Repeat Q1 <- Q, Q <- LINK(Q) if ABC(P) < ABC(Q)
      while((P->ABC < 0? -1: P->ABC + M->ABC) < Q->ABC) {
        Q1 = Q;
        Q = Q->LINK;
      }

// M2 [Multiply cycle] replace "ABC(P)" with "(if ABC(P) < 0 then -1, otherwise ABC(P) + ABC(M))"
// A3 [Add coefficients] Found terms with equal exponents
      if((P->ABC < 0? -1: P->ABC + M->ABC) == Q->ABC) {

// A3 [Add coefficients] Terminate if ABC(P) < 0
        if(P->ABC < 0)
          break;

// M2 [Multiply cycle] replace "COEF(P)" with "COEF(P) x COEF(M)"
// A3 [Add coefficients] COEF(Q) <- COEF(Q) + COEF(P)
        Q->COEF += P->COEF * M->COEF;

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

// M2 [Multiply cycle] replace "COEF(P)" with "COEF(P) x COEF(M)"
// A5 [Insert new term] COEF(Q2) <- COEF(P), ABC(Q2) <- ABC(P), ...
      Q2->COEF = P->COEF * M->COEF;

// M2 [Multiply cycle] replace "ABC(P)" with "(if ABC(P) < 0 then -1, otherwise ABC(P) + ABC(M))"
      Q2->ABC = (P->ABC < 0? -1: P->ABC + M->ABC);

      Q2->LINK = Q;
      Q1->LINK = Q2;
      Q1 = Q2;
      P = P->LINK;
// A5 [Insert new term] To A2
    }

  }

  return 0;

}

void cleanup(struct Term* Q, struct Term* M, struct Term* P)
{
  polynomial_clear(P);
  polynomial_clear(M);
  polynomial_clear(Q);
}

int main(int argc, char* argv[])
{

  struct Term Q = {NULL, -1, 0};
  struct Term M = {NULL, -1, 0};
  struct Term P = {NULL, -1, 0};

  struct Term* polys[] = {&Q, &M, &P};
  if(get_polynomials(stdin, polys, 3) != 0) {
    cleanup(&Q, &M, &P);
    return 1;
  }

  if(Mult(&Q, &M, &P) != 0) {
    cleanup(&Q, &M, &P);
    return 2;
  }

  polynomial_write(&Q, stdout);

  cleanup(&Q, &M, &P);

  return 0;
}

