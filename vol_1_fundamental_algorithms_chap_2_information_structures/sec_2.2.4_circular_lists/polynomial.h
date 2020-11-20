#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
// polynomial.h

// Algorithm A (Addition of polynomials), 2.2.4 Circular Lists,
// The Art of Computer Programming, Donald Knuth

// Program A (Addition of polynomials), 2.2.4 Circular Lists,
// The MMIX Supplement, Martin Ruckert

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct Term {
  struct Term* LINK;
  int64_t ABC;
  int64_t COEF;
};

int get_polynomials(FILE* in, struct Term* polys[], uint32_t npolys);
void polynomial_write(const struct Term* P, FILE* out);
void polynomial_clear(struct Term* P);

#endif

